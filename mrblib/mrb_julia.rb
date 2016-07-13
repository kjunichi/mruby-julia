class Julia
    def self.export_mrb
        jl = <<'EOS'
function set_mrb_state(ptr::Ptr{Void})
  global MRB_MRB_STATE = ptr
end
EOS
        Julia.eval(jl)

        jl = <<'EOS'
function mrb_test()
  r = ccall( (:mrb_load_string, "libmruby"), Void, (Ptr{Void},Cstring), MRB_MRB_STATE, "puts 'Hello,World!'")
end
EOS
        Julia.eval(jl)
        Julia.export_mrb_state
    end
end
