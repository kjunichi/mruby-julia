class Julia
    def self.export_mrb
        jl = <<'EOS'
function set_mrb_state(ptr::Ptr{Void})
  global MRB_MRB_STATE = ptr
end
EOS
        Julia.eval(jl)

        jl = <<'EOS'
type MRB　# type < 0.6
    _::NTuple{16, Cchar} 
end

function mrb_test()
  
  r = ccall( (:mrb_load_string, "mruby"), MRB, (Ptr{Void},Cstring), MRB_MRB_STATE, "puts 'Hello,World!'")
  
end
EOS
        Julia.eval(jl)
        Julia.export_mrb_state
    end
end
