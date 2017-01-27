/*
** mrb_julia.c - Julia class
**
** Copyright (c) Junichi Kajiwara 2016
**
** See Copyright Notice in LICENSE
*/
#include <julia.h>

#include "mruby.h"
#include "mruby/data.h"
#include "mrb_julia.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
  char *str;
  int len;
} mrb_julia_data;

static const struct mrb_data_type mrb_julia_data_type = {
  "mrb_julia_data", mrb_free,
};

static mrb_value
mrb_julia_init(mrb_state *mrb, mrb_value self)
{
  mrb_julia_data *data;

  data = (mrb_julia_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_julia_data_type;
  DATA_PTR(self) = NULL;

  // mrb_get_args(mrb, "s", &str, &len);
  data = (mrb_julia_data *)mrb_malloc(mrb, sizeof(mrb_julia_data));
  // data->str = str;
  data->len = 0;
  DATA_PTR(self) = data;

  return self;
}

static mrb_value
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

static mrb_value
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
mrb_mruby_julia_gem_init(mrb_state *mrb)
{
  struct RClass *julia;
  julia = mrb_define_class(mrb, "Julia", mrb->object_class);
  mrb_define_method(mrb, julia, "initialize", mrb_julia_init, MRB_ARGS_NONE());
  mrb_define_method(mrb, julia, "eval", mrb_julia_eval, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, julia, "eval", mrb_julia_eval, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, julia, "export_mrb_state", mrb_export_mrb_state, MRB_ARGS_NONE());
  //jl_init(JULIA_INIT_DIR "/../bin");
  jl_init("C:\\Users\\kjw_j\\AppData\\Local\\Julia-0.5.0\\bin");
  DONE;
}

void
mrb_mruby_julia_gem_final(mrb_state *mrb)
{
  /* fprintf(stderr, "mrb_mruby_julia_gem_final: start\n"); */
  jl_atexit_hook(0);
  /* fprintf(stderr, "mrb_mruby_julia_gem_final: end\n"); */
}
