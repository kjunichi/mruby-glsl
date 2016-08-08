class Glsl
  attr_reader :vertexShader
  attr_reader :fragmentShader
  attr_reader :width
  attr_reader :height

  def initialize
    @width = 256
    @height = 256
  end

  def size(width, heigth)
    @width = width
    @height = height
  end

  def attachVertexShader(shader)
    @vertexShader = shader
  end

  def attachFragmentShader(shader)
    @fragmentShader = shader
  end

end
