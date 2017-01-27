#include <julia.h>

#include "mruby.h"
#include "mruby/data.h"
#include "mrb_julia.h"


MRB_BEGIN_DECL
mrb_value
mrb_julia_eval(mrb_state *mrb, mrb_value self);

mrb_value
mrb_export_mrb_state(mrb_state *mrb, mrb_value self);

void
mrb_mruby_julia_gem_final(mrb_state *mrb);

void
mrb_jl_init(void);
MRB_END_DECL

mrb_value
mrb_julia_eval(mrb_state *mrb, mrb_value self)
{
  char *str;
  char *juliaScript;
  char *returnedString;
  int len;
  jl_value_t *ret;

  mrb_get_args(mrb, "s", &str, &len);
  juliaScript = (char *)malloc(sizeof(char) * (len + 1));
  memcpy(juliaScript, str, len);
  juliaScript[len] = '\0';

  /* run julia commands */
  ret = jl_eval_string(juliaScript);
  if (jl_exception_occurred()) {
    fprintf(stderr, "julia exception occurred!\n");
    jl_show(jl_stderr_obj(), jl_exception_occurred());
    jl_printf(jl_stderr_stream(), "\n");
    return mrb_nil_value();
  }
#if JULIA_VERSION_MAJOR == 0 && JULIA_VERSION_MINOR < 5
    if (jl_is_byte_string(ret)) {
#else
    if (jl_is_string(ret)) {
#endif

#if JULIA_VERSION_MAJOR == 0 && JULIA_VERSION_MINOR < 5
    returnedString = (char *)jl_bytestring_ptr(ret);
#else
    returnedString = (char *)jl_string_ptr(ret);
#endif
    
    return mrb_str_new(mrb, returnedString, strlen(returnedString));
  }
  return mrb_nil_value();
}

mrb_value
mrb_export_mrb_state(mrb_state *mrb, mrb_value self)
{
  jl_function_t *func;
  jl_value_t *argument;
  jl_value_t *ret;

  func = jl_get_function(jl_main_module, "set_mrb_state");
  argument = jl_box_voidpointer((void *)mrb);
  ret = jl_call1(func, argument);
  if (jl_exception_occurred()) {
    fprintf(stderr, "julia exception occurred!\n");
    return mrb_nil_value();
  }
  return self;
}

void
mrb_mruby_julia_gem_final(mrb_state *mrb)
{
  /* fprintf(stderr, "mrb_mruby_julia_gem_final: start\n"); */
  jl_atexit_hook(0);
  /* fprintf(stderr, "mrb_mruby_julia_gem_final: end\n"); */
}

void
mrb_jl_init()
{
#ifndef _WIN32
  jl_init(JULIA_INIT_DIR "/../bin");
#else
  jl_init(JULIA_INIT_DIR "\\..\\bin");
#endif
}

