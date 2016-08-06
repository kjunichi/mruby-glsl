vshader = <<'EOT'
#version 120
#
#ifdef GL_ES
precision mediump float;
#endif
attribute vec3 position;
void main() {
  gl_Position = vec4( position, 1.0 );
}
EOT

fshader = <<'EOT'
#ifdef GL_ES
precision mediump float;
#endif
#extension GL_OES_standard_derivatives : enable

uniform vec2 resolution;

vec3 getColor(int n) {
  vec3 c = vec3(
    1.0/(mod(float(n),11.0)),
    1.0/(mod(float(n),13.0)),
    1.0/(mod(float(n),19.0))
  );
  return c;
}
void main() {
  int num;
  vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / min(resolution.x,resolution.y);
  gl_FragColor = vec4(0.0, 0.0,0.0,1.0);
  vec2 zz = vec2(0.0,0.0);
  num=0;
  pos = pos + vec2(-0.5, 0.0);
  for(int i=0; i < 50; i++) {
    num++;
    zz = vec2((zz.x * zz.x) - (zz.y * zz.y), 2.0 * zz.x * zz.y) + pos;
    if(length(zz)>10.0) {
      break;
    }
  }

  gl_FragColor = vec4(getColor(num),1.0);

}
EOT


glsl = Glsl.new
glsl.attachVertexShader vshader
glsl.attachFragmentShader fshader

puts glsl.render
