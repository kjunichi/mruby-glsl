class Glsl
  attr_reader :vertexShader
  attr_reader :fragmentShader

  def attachVertexShader(shader)
    @vertexShader = shader
  end

  def attachFragmentShader(shader)
    @fragmentShader = shader
  end

end
