/*
** mrb_julia.c - Julia class
**
** Copyright (c) Junichi Kajiwara 2016
**
** See Copyright Notice in LICENSE
*/
// #include <julia.h>

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

extern mrb_value
mrb_julia_eval(mrb_state *mrb, mrb_value self);

extern mrb_value
mrb_export_mrb_state(mrb_state *mrb, mrb_value self);

extern void
mrb_mruby_julia_gem_final(mrb_state *mrb);

extern void
mrb_jl_init(void);

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

void
mrb_mruby_julia_gem_init(mrb_state *mrb)
{
  struct RClass *julia;
  julia = mrb_define_class(mrb, "Julia", mrb->object_class);
  mrb_define_method(mrb, julia, "initialize", mrb_julia_init, MRB_ARGS_NONE());
  mrb_define_method(mrb, julia, "eval", mrb_julia_eval, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, julia, "eval", mrb_julia_eval, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, julia, "export_mrb_state", mrb_export_mrb_state, MRB_ARGS_NONE());
  
  mrb_jl_init();

  DONE;
}

