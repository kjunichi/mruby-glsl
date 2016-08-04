##
## Glsl Test
##

assert("Glsl#hello") do
  t = Glsl.new "hello"
  assert_equal("hello", t.hello)
end

assert("Glsl#bye") do
  t = Glsl.new "hello"
  assert_equal("hello bye", t.bye)
end

assert("Glsl.hi") do
  assert_equal("hi!!", Glsl.hi)
end
