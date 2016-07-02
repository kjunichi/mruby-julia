/*
** mrb_julia.c - Julia class
**
** Copyright (c) Junichi Kajiwara 2016
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mrb_julia.h"

#include <julia.h>

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
  char *str;
  int len;
} mrb_julia_data;

static const struct mrb_data_type mrb_julia_data_type = {
  "mrb_julia_data", mrb_free,
};

static mrb_value mrb_julia_init(mrb_state *mrb, mrb_value self)
{
  mrb_julia_data *data;

  data = (mrb_julia_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_julia_data_type;
  DATA_PTR(self) = NULL;

  //mrb_get_args(mrb, "s", &str, &len);
  data = (mrb_julia_data *)mrb_malloc(mrb, sizeof(mrb_julia_data));
  //data->str = str;
  data->len = 0;
  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_julia_eval(mrb_state *mrb, mrb_value self)
{
  char *str;
  char *juliaScript;
  char *returnedString;
  int len;

  mrb_julia_data *data = DATA_PTR(self);
  mrb_get_args(mrb, "s", &str, &len);
  juliaScript = (char*)malloc(sizeof(char)*(len+1));
  memcpy(juliaScript, str, len);
  juliaScript[len]='\0';

  /* run julia commands */
  jl_value_t *ret = jl_eval_string(juliaScript);
  if(jl_is_byte_string(ret)) {
    //fprintf(stderr,"jl_is_string == true!\n");
    returnedString = (char*)jl_bytestring_ptr(ret);
    data->str = returnedString;
    //data->len = jl_array_len((jl_array_t*)ret);
    data->len=strlen(returnedString);
    //fprintf(stderr,"len = %d\n",data->len);
    data->str[data->len] = '\0';
  }


  return mrb_str_new(mrb, data->str, data->len);
}

void mrb_mruby_julia_gem_init(mrb_state *mrb)
{
    struct RClass *julia;
    julia = mrb_define_class(mrb, "Julia", mrb->object_class);
    mrb_define_method(mrb, julia, "initialize", mrb_julia_init, MRB_ARGS_NONE());
    mrb_define_method(mrb, julia, "eval", mrb_julia_eval, MRB_ARGS_REQ(1));
    //mrb_define_class_method(mrb, julia, "hi", mrb_julia_hi, MRB_ARGS_NONE());
    jl_init(JULIA_INIT_DIR);
    DONE;
}

void mrb_mruby_julia_gem_final(mrb_state *mrb)
{
  fprintf(stderr,"mrb_mruby_julia_gem_final: start\n");
  jl_atexit_hook(0);
  fprintf(stderr,"mrb_mruby_julia_gem_final: end\n");

}
