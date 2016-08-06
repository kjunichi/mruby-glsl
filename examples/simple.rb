vshader = <<'EOT'
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

void main(void){
      gl_FragColor = vec4(0.0, 0.6, 0.4, 1.0);
}
EOT


glsl = Glsl.new
glsl.attachVertexShader vshader
glsl.attachFragmentShader fshader

puts glsl.render
