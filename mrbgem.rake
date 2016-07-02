MRuby::Gem::Specification.new('mruby-julia') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Junichi Kajiwara'

  spec.cc.flags << `/Applications/Julia-0.4.5.app/Contents/Resources/julia/share/julia/julia-config.jl --cflags`.chomp
  #spec.linker.library_paths << "/Applications/Julia-0.4.5.app/Contents/Resources/julia/lib/julia"
  #spec.linker.libraries << "julia"
  #spec.linker.flags_before_libraries << `/Applications/Julia-0.4.5.app/Contents/Resources/julia/share/julia/julia-config.jl --ldflags --ldlibs`
  #spec.linker.flags << "-L/Applications/Julia-0.4.5.app/Contents/Resources/julia/lib/julia -Wl,-rpath,/Applications/Julia-0.4.5.app/Contents/Resources/julia/lib/julia -ljulia"
  spec.linker.flags << `/Applications/Julia-0.4.5.app/Contents/Resources/julia/share/julia/julia-config.jl --ldflags --ldlibs`.gsub!(/\n/,' ')
end
