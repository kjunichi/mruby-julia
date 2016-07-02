##
## Julia Test
##

assert("Julia#eval") do
  t = Julia.new
  s = "sin(pi / 4) - sin(pi / 4))"
  assert_equal("0.0", t.eval(s))
end
