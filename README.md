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
p Glsl.hi
#=> "hi!!"
t = Glsl.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
