MRuby::Gem::Specification.new('mruby-julia') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Junichi Kajiwara'
  p RUBY_PLATFORM
  p ENV['VisualStudioVersion']
  p ENV['VSINSTALLDIR']
  if RUBY_PLATFORM =~ /darwin/i
    juliaPath = `which julia`
    if(juliaPath.length>0)
      juliaPath=Pathname(juliaPath).parent.parent.to_s
      juliaConfig = juliaPath+"/share/julia/julia-config.jl"
    else
      #juliaConfit = "/Applications/Julia-0.4.5.app/Contents/Resources/julia/share/julia/julia-config.jl"
      juliaApp = `ls /Applications/|grep ^Julia|sort -r`.split("\n")[0]
      juliaConfig="/Applications/#{juliaApp}/Contents/Resources/julia/bin/julia /Applications/#{juliaApp}/Contents/Resources/julia/share/julia/julia-config.jl"
    end
    spec.cc.flags << `#{juliaConfig} --cflags`.chomp
    spec.linker.flags << `#{juliaConfig} --ldflags --ldlibs`.gsub!(/\n/,' ')
  elsif ENV['VisualStudioVersion'] || ENV['VSINSTALLDIR']
    JULIA_HOME="#{ENV['USERPROFILE']}\\appdata\\local\\julia-0.5.0\\bin"
    JULIA_INC = "#{JULIA_HOME}\\..\\include\\julia"
    #spec.cc.flags << "/TP /I #{JULIA_INC}"
    spec.cxx.flags << "/I #{JULIA_INC} /DJULIA_INIT_DIR=#{JULIA_HOME}"
    #spec.linker.flags << "/L:#{JULIA_HOME}\\..\\lib"
    spec.linker.libraries << 'libjulia'
  else
    # Linux
    juliaPath=Pathname(`which julia`).parent.parent.to_s
    juliaConfig = juliaPath+"/share/julia/julia-config.jl"
    #spec.cc.flags << `#{juliaConfig} --cflags`.chomp
    spec.cxx.flags << `#{juliaConfig} --cflags`.chomp
    spec.linker.flags << `#{juliaConfig} --ldflags --ldlibs`.gsub!(/\n/,' ')
    spec.linker.flags << "-Wl,-rpath #{juliaPath}/lib/julia"
    spec.linker.libraries << 'julia'
  end
end
