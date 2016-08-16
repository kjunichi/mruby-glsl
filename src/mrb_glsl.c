/*
** mrb_glsl.c - Glsl class
**
** Copyright (c) @kjunichi 2016
**
** See Copyright Notice in LICENSE
*/
#ifndef _WIN32
#include <sys/time.h>
#else
#include <windows.h>
#endif
#include "mruby.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "mruby/string.h"

#include "mrb_glsl.h"
#include <stdlib.h>
#include <GLFW/glfw3.h>

#define DONE mrb_gc_arena_restore(mrb, 0);

extern int
glslInit();

extern void
glslTerminate();

extern int
render_image(mrb_state *mrb, mrb_value obj, GLFWwindow *, double, unsigned char **);

typedef struct {
  char *str;
  int len;
  GLFWwindow *window;
#ifndef _WIN32
  struct timeval start_tv, tv;
#else
  SYSTEMTIME start_st, st;
#endif
} mrb_glsl_data;

static void
mrb_glsl_free(mrb_state *, void *);

static const struct mrb_data_type mrb_glsl_data_type = {
  "mrb_glsl_data", mrb_glsl_free,
};

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

static mrb_value
mrb_glsl_init(mrb_state *mrb, mrb_value self)
{
  mrb_glsl_data *data;
  GLFWwindow *window;

  fprintf(stderr, "init start.\n");
  data = (mrb_glsl_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  data = (mrb_glsl_data *)mrb_malloc(mrb, sizeof(mrb_glsl_data));
  data->window = NULL;
  DATA_TYPE(self) = &mrb_glsl_data_type;
  DATA_PTR(self) = data;

  glfwWindowHint(GLFW_VISIBLE, 0);
  // getWindowSize(mrb, self, &width, &height);
  window = glfwCreateWindow(256, 256, "mruby glsl", NULL, NULL);
  if (!window) {
    glfwTerminate();
    mrb_full_gc(mrb);
    window = glfwCreateWindow(256, 256, "mruby glsl", NULL, NULL);
    if (!window) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "Oops glfwCreateWindow fail!");
    }
  }
  data->window = window;
#ifndef _WIN32
  gettimeofday(&(data->start_tv), NULL);
#else
  GetSystemTime( &(data->start_st) );
#endif
  return self;
}

static mrb_value
mrb_glsl_set_size(mrb_state *mrb, mrb_value self)
{
  int width, height;
  mrb_glsl_data *data;
  GLFWwindow *window;

  fprintf(stderr, "set_size start.\n");
  getWindowSize(mrb, self, &width, &height);
  fprintf(stderr, "w,h = %d, %d\n", width, height);
  data = (mrb_glsl_data *)DATA_PTR(self);
  window = data->window;
  if (window != NULL) {
    glfwSetWindowSize(window, width, height);
  }
  fprintf(stderr, "set_size end.\n");

  return mrb_nil_value();
}

static mrb_value
mrb_glsl_hi(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, "hi!!");
}

static mrb_value
mrb_glsl_render(mrb_state *mrb, mrb_value self)
{
  GLFWwindow *window;
  unsigned char *buf = NULL;
  int size;
  mrb_value ppmImage;
  mrb_glsl_data *data;
  double t;

  data = (mrb_glsl_data *)DATA_PTR(self);
  window = data->window;
#ifndef _WIN32
  gettimeofday(&(data->tv), NULL);
  t = (data->tv.tv_usec - data->start_tv.tv_usec) / 1000000.0;
  t = t+(data->tv.tv_sec - data->start_tv.tv_sec);
#else
  GetSystemTime( &(data->st) );
  t = (data->st.wMilliseconds - data->start_st.wMilliseconds) / 1000.0;
#endif
  size = render_image(mrb, self, window, t, &buf);
  ppmImage = mrb_str_new(mrb, (const char *)buf, size);
  return ppmImage;
}

static void
mrb_glsl_free(mrb_state *mrb, void *ptr)
{
  mrb_glsl_data *h = ptr;
  if (h->window != NULL) {
    glfwDestroyWindow(h->window);
  }

  mrb_free(mrb, h);
}

void
mrb_mruby_glsl_gem_init(mrb_state *mrb)
{
  struct RClass *glsl;
  if (glslInit() != 0) {
    // fprintf(stderr, "Can't init glsls!");
    // exit(1);
    mrb_raise(mrb, E_RUNTIME_ERROR, "Can't init glsls!");
  }
  glsl = mrb_define_class(mrb, "Glsl", mrb->object_class);
  mrb_define_method(mrb, glsl, "initialize", mrb_glsl_init, MRB_ARGS_NONE());
  mrb_define_method(mrb, glsl, "hi", mrb_glsl_hi, MRB_ARGS_NONE());
  mrb_define_method(mrb, glsl, "setSize", mrb_glsl_set_size, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, glsl, "render", mrb_glsl_render, MRB_ARGS_NONE());
  DONE;
}

void
mrb_mruby_glsl_gem_final(mrb_state *mrb)
{
  glslTerminate();
}
