##
## Glsl Test
##

assert("Glsl#attachVertexShader") do
  t = Glsl.new
  t.attachVertexShader("hogehoge")
  assert_equal("hogehoge", t.vertexShader)
end

assert("Glsl#attachFragmentShader") do
  t = Glsl.new 
  t.attachFragmentShader("foo bar")
  assert_equal("foo bar", t.fragmentShader)
end
