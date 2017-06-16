MRuby::Build.new do |conf|
  toolchain :visualcpp
  conf.gembox 'default'
  conf.gem '../mruby-julia'
  conf.enable_test
end
