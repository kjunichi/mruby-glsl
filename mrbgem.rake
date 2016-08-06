MRuby::Gem::Specification.new('mruby-glsl') do |spec|
  spec.license = 'MIT'
  spec.authors = '@kjunichi'
  if RUBY_PLATFORM =~ /darwin/i
    spec.cc.flags << '-I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers'
    spec.linker.flags << '-framework OpenGL -lglfw3'
  end
  if RUBY_PLATFORM =~ /linux/i
    spec.linker.libraries << 'glfw'
    spec.linker.libraries << 'GLEW'
    spec.linker.libraries << 'GLU'
    spec.linker.libraries << 'GL'
  end

  case RUBY_PLATFORM
  when /mswin|mingw/
    spec.linker.libraries << 'glfw3'
    spec.linker.libraries << 'glew32'
    spec.linker.libraries << 'glu32'
    spec.linker.libraries << 'opengl32'
    spec.linker.libraries << 'user32'
    spec.linker.libraries << 'Advapi32'
    spec.linker.libraries << 'gdi32'
    spec.linker.libraries << 'shell32'
  end
end
