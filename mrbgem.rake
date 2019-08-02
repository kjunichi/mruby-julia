MRuby::Gem::Specification.new('mruby-julia') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Junichi Kajiwara'
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
    spec.cxx.flags << `#{juliaConfig} --cflags`.chomp.gsub!('std=gnu99','std=c++11')
    spec.linker.flags << `#{juliaConfig} --ldflags --ldlibs`.gsub!(/\n/,' ')
  elsif ENV['VisualStudioVersion'] || ENV['VSINSTALLDIR']
    JN=`dir /b /ad #{ENV['USERPROFILE']}\\AppData\\Local\\Julia-*`.split("\n")[-1]
    JULIA_HOME="#{ENV['USERPROFILE']}\\appdata\\local\\#{JN}\\bin"
    JULIA_INC = "#{JULIA_HOME}\\..\\include\\julia"
    tmp = JULIA_HOME
    tmp.gsub!(/\\/, "/")
    p "#{JULIA_HOME}"
    JLH=JULIA_HOME.gsub(/\//,"\\")
    #`dumpbin /exports #{JLH}\\libjulia.dll > #{JLH}\\..\\lib\\libjulia.tmp`
    #dump = `type #{JLH}\\..\\lib\\libjulia.tmp`
    #defFile=[]
    # dump.split("\n").each{|line|
    #     items = line.split(" ")
    #     funcName = items[3]
    #     if funcName!=nil then
    #       line = "#{funcName}"
    #       if items[4]=='=' then
    #         line = line + " = " + items[5]
    #       end  
    #       defFile.push "#{line}"
    #     end
    # }
    # defFilePath = "#{JLH}\\..\\lib\\libjulia.def"
    # #p defFilePath
    # defFile = ["EXPORTS",defFile.slice(8,defFile.length)]
    # File.write(defFilePath, defFile.join("\n"))
    # libPath = "#{JLH}\\..\\lib\\libjulia.lib"
    # p `LIB /DEF:#{defFilePath} /MACHINE:X64 /out:#{libPath}`
    spec.cxx.flags << "/I #{JULIA_INC} /DJULIA_INIT_DIR=\\\"#{tmp}\\\""
    spec.linker.flags << "/LIBPATH:#{JULIA_HOME}\\..\\lib"
    p `copy #{JLH}\\..\\lib\\libjulia.dll.a #{JLH}\\..\\lib\\libjulia.dll.lib`
    p `copy #{JLH}\\..\\lib\\libopenlibm.dll.a #{JLH}\\..\\lib\\libopenlibm.dll.lib`
    spec.linker.libraries << 'libjulia.dll'
    spec.linker.libraries << 'libopenlibm.dll'
  else
    # Linux
    juliaPath=Pathname(`which julia`).parent.parent.to_s
    juliaConfig = juliaPath+"/share/julia/julia-config.jl"
    #spec.cc.flags << `#{juliaConfig} --cflags`.chomp
    spec.cxx.flags << `#{juliaConfig} --cflags`.chomp.gsub!('std=gnu99','std=c++11')
    spec.linker.flags << `#{juliaConfig} --ldflags --ldlibs`.gsub!(/\n/,' ')
    spec.linker.flags << "-Wl,-rpath #{juliaPath}/lib/julia"
    spec.linker.flags << "#{juliaPath}/lib/julia/libstdc++.so.6"
    spec.linker.libraries << 'julia'
  end
end
