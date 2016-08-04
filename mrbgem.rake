MRuby::Gem::Specification.new('mruby-glsl') do |spec|
  spec.license = 'MIT'
  spec.authors = '@kjunichi'

  spec.cc.flags << '-I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers'
  spec.linker.flags << '-framework OpenGL -lglfw3'
end
