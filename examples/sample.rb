io = File.open("simple.vert", "r")
glsl = Glsl.new
glsl.attachVertexShader(io.read)
io.close
io = File.open("simple.frag", "r")
glsl.attachFragmentShader(io.read)
io.close
#p glsl.vertexShader
#
#img = glsl.render
#
#f = File.open('glsl.ppm','w')
#f.write img
#f.close
