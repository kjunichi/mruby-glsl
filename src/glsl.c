#ifndef __APPLE__
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#ifdef __APPLE__
#include <glu.h>
#else
#include <GL/glu.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/string.h"

int gWidth;
int gHeight;

int gDone = 0;
GLint resolutionLoc;

/*
** 光源
*/
static const GLfloat lightpos[] = {0.0f, 0.0f, 5.0f, 1.0f}; /* 位置　　　 */
static const GLfloat lightcol[] = {1.0f, 1.0f, 1.0f, 1.0f}; /* 直接光強度 */
static const GLfloat lightamb[] = {0.1f, 0.1f, 0.1f, 1.0f}; /* 環境光強度 */

/*
** シェーダ
*/

static GLuint vertShader;
static GLuint fragShader;
static GLuint gl2Program;

static void
initgl(mrb_state *mrb, mrb_value obj)
{
  GLint compiled, linked;
  mrb_value shader;
  GLsizei len;
#ifndef __APPLE__
  GLenum err;
#endif

#ifndef __APPLE__
  err = glewInit();
#endif
  fprintf(stderr, "initgl start\n");
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  /* 光源の初期設定 */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

  fprintf(stderr, "initgl before glCreateShader\n");
  /* シェーダオブジェクトの作成 */
  vertShader = glCreateShader(GL_VERTEX_SHADER);
  fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  fprintf(stderr, "initgl after glCreateShader\n");
  /* シェーダのソースプログラムの読み込み */
  // if (readShaderSource(vertShader, "simple.vert"))
  //  exit(1);

  shader = mrb_iv_get(mrb, obj, mrb_intern_lit(mrb, "@vertexShader"));
  /*mrb_funcall(mrb, mrb_top_self(mrb), "p", 1, shader);*/
  len = RSTRING_LEN(shader);
  // fprintf(stderr, "len = %d\n", len);
  const char *source = RSTRING_PTR(shader);
  glShaderSource(vertShader, 1, &source, &len);
  fprintf(stderr, "vertShader done.\n");

  shader = mrb_iv_get(mrb, obj, mrb_intern_lit(mrb, "@fragmentShader"));
  len = RSTRING_LEN(shader);
  source = RSTRING_PTR(shader);
  glShaderSource(fragShader, 1, &source, &len);
  fprintf(stderr, "fragShader done.\n");

  /* バーテックスシェーダのソースプログラムのコンパイル */
  glCompileShader(vertShader);
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
  // printShaderInfoLog(vertShader);
  if (compiled == GL_FALSE) {
    fprintf(stderr, "Compile error in vertex shader.\n");
    exit(1);
  }

  /* フラグメントシェーダのソースプログラムのコンパイル */
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
  // printShaderInfoLog(fragShader);
  if (compiled == GL_FALSE) {
    fprintf(stderr, "Compile error in fragment shader.\n");
    exit(1);
  }

  /* プログラムオブジェクトの作成 */
  gl2Program = glCreateProgram();

  /* シェーダオブジェクトのシェーダプログラムへの登録 */
  glAttachShader(gl2Program, vertShader);
  glAttachShader(gl2Program, fragShader);

  /* シェーダオブジェクトの削除 */
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  /* シェーダプログラムのリンク */
  glLinkProgram(gl2Program);
  glGetProgramiv(gl2Program, GL_LINK_STATUS, &linked);
  // printProgramInfoLog(gl2Program);
  if (linked == GL_FALSE) {
    fprintf(stderr, "Link error.\n");
    exit(1);
  }

  fprintf(stderr, "resolutionLoc = %d\n", resolutionLoc);
  /* シェーダプログラムの適用 */
  glUseProgram(gl2Program);

  resolutionLoc = glGetUniformLocation(gl2Program, "resolution");
}

static void
scene()
{
  static const GLfloat diffuse[] = {0.6f, 0.1f, 0.1f, 1.0f};
  static const GLfloat specular[] = {0.3f, 0.3f, 0.3f, 1.0f};

  /* 材質の設定 */
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0f);

#if 1
  /* １枚の４角形を描く */
  glNormal3d(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glVertex3d(-1.0, -1.0, 0.0);
  glVertex3d(1.0, -1.0, 0.0);
  glVertex3d(1.0, 1.0, 0.0);
  glVertex3d(-1.0, 1.0, 0.0);
  glEnd();
#else
  glutSolidTeapot(1.0);
#endif
}

int
getppm(unsigned char **ppm)
{
  int x, y, pos;
  unsigned char *ppmImage;
  GLubyte *dataBuffer = NULL;
  unsigned char linebuf[3];
  fprintf(stderr, "getppm start\n");
  ppmImage = (GLubyte *)malloc(gWidth * gHeight * 3 + 512);

  pos = snprintf((char *)ppmImage, 4, "P6\n");
  pos += snprintf((char *)(ppmImage + pos), 32, "%d %d\n", gWidth, gHeight);
  pos += snprintf((char *)(ppmImage + pos), 5, "255\n");
  fprintf(stderr, "pos = %d\n", pos);
  dataBuffer = &(ppmImage[pos]);

  glReadPixels(0, 0, gWidth, gHeight, GL_RGB, GL_UNSIGNED_BYTE, dataBuffer);

  for (y = 0; y < gHeight / 2; y++) {
    for (x = 0; x < gWidth; x++) {
      linebuf[0] = dataBuffer[3 * (x + gWidth * y)];
      linebuf[1] = dataBuffer[3 * (x + gWidth * y) + 1];
      linebuf[2] = dataBuffer[3 * (x + gWidth * y) + 2];
      dataBuffer[3 * (x + gWidth * y)] = dataBuffer[3 * (x + gWidth * (gHeight - 1 - y))];
      dataBuffer[3 * (x + gWidth * y) + 1] = dataBuffer[3 * (x + gWidth * (gHeight - 1 - y)) + 1];
      dataBuffer[3 * (x + gWidth * y) + 2] = dataBuffer[3 * (x + gWidth * (gHeight - 1 - y)) + 2];
      dataBuffer[3 * (x + gWidth * (gHeight - 1 - y))] = linebuf[0];
      dataBuffer[3 * (x + gWidth * (gHeight - 1 - y)) + 1] = linebuf[1];
      dataBuffer[3 * (x + gWidth * (gHeight - 1 - y)) + 2] = linebuf[2];
    }
  }
  *ppm = ppmImage;
  return pos + 3 * gWidth * gHeight;
}

static int
display(unsigned char **ppmImage)
{
  fprintf(stderr, "display start\n");

  /* モデルビュー変換行列の初期化 */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* 画面クリア */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* 光源の位置を設定 */
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  /* 視点の移動（物体の方を奥に移動）*/
  glTranslated(0.0, 0.0, -1.0);

  /* シーンの描画 */
  scene();
  return getppm(ppmImage);
}
void
getWindowSize(mrb_state *mrb, mrb_value obj, int *width, int *height)
{
  mrb_value value;

  value = mrb_iv_get(mrb, obj, mrb_intern_lit(mrb, "@width"));
  if (mrb_fixnum_p(value)) {
    *width = mrb_fixnum(value);
  } else {
    *width = -1;
  }
  value = mrb_iv_get(mrb, obj, mrb_intern_lit(mrb, "@height"));
  if (mrb_fixnum_p(value)) {
    *height = mrb_fixnum(value);
  } else {
    *height = -1;
  }
}

int
render_image(mrb_state *mrb, mrb_value obj, unsigned char **ppmImage)
{
  GLFWwindow *window;
  int size = -1, width, height;

  if (!glfwInit())
    return -1;

  // OpenGL Version 3.2 Core Profile を選択する
  /*
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
*/
  glfwWindowHint(GLFW_VISIBLE, 0);
  getWindowSize(mrb, obj, &width, &height);
  window = glfwCreateWindow(width, height, "Simple example", NULL, NULL);
  if (!window) {
    glfwTerminate();
    printf("Oops glfwCreateWindow¥n");
    return -1;
  }

  glfwMakeContextCurrent(window);
  initgl(mrb, obj);
  if (!glfwWindowShouldClose(window)) {
    int w, h;

    glfwGetFramebufferSize(window, &w, &h);
    gWidth = w;
    gHeight = h;

    glUniform2f(resolutionLoc, gWidth, gHeight);
    // glUniform2f(glGetUniformLocation(gl2Program, "resolution"), gWidth, gHeight);
    glViewport(0, 0, w, h);
    // OpenGLでの描画処理をここに書く
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, w, h);

    /* 透視変換行列の指定 */
    glMatrixMode(GL_PROJECTION);

    /* 透視変換行列の初期化 */
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* 画面クリア */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* 光源の位置を設定 */
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    /* 視点の移動（物体の方を奥に移動）*/
    glTranslated(0.0, 0.0, -1.0);

    fprintf(stderr, "before display\n");
    size = display(ppmImage);
  }
  glfwTerminate();
  return size;
}
