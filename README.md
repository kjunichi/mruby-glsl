# mruby-glsl   [![Build Status](https://travis-ci.org/kjunichi/mruby-glsl.svg?branch=master)](https://travis-ci.org/kjunichi/mruby-glsl)
Glsl class

## install by mrbgems

- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'kjunichi/mruby-glsl'
end
```

## example

```ruby
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
#version 120
#ifdef GL_ES
precision mediump float;
#endif

void main(void){
      gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
EOT


glsl = Glsl.new
glsl.attachVertexShader vshader
glsl.attachFragmentShader fshader

puts glsl.render
```

## License
under the MIT License:
- see LICENSE file
