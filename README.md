# mruby-julia   [![Build Status](https://travis-ci.org/kjunichi/mruby-julia.svg?branch=master)](https://travis-ci.org/kjunichi/mruby-julia) [![Build status](https://ci.appveyor.com/api/projects/status/3ooump02iafh3v2t/branch/master?svg=true)](https://ci.appveyor.com/project/kjunichi/mruby-julia/branch/master)

## example

```ruby
j = Julia.new
j.eval("string(sqrt(2.0))")
```

## install by mrbgems

- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'kjunichi/mruby-julia'
end
```

## License
under the MIT License:
- see LICENSE file
