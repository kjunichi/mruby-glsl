/*
** mrb_glsl.c - Glsl class
**
** Copyright (c) @kjunichi 2016
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mrb_glsl.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

extern int
render_image(mrb_state *mrb, mrb_value obj, unsigned char **);

typedef struct {
  char *str;
  int len;
} mrb_glsl_data;

static const struct mrb_data_type mrb_glsl_data_type = {
  "mrb_glsl_data", mrb_free,
};

static mrb_value
mrb_glsl_init(mrb_state *mrb, mrb_value self)
{
  mrb_glsl_data *data;

  data = (mrb_glsl_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_glsl_data_type;
  DATA_PTR(self) = NULL;

  return self;
}

static mrb_value
mrb_glsl_hi(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, "hi!!");
}

static mrb_value
mrb_glsl_render(mrb_state *mrb, mrb_value self)
{
  unsigned char *buf = NULL;
  int size;

  mrb_value ppmImage;
  size = render_image(mrb, self, &buf);
  ppmImage = mrb_str_new(mrb, (const char *)buf, size);
  return ppmImage;
}

void
mrb_mruby_glsl_gem_init(mrb_state *mrb)
{
  struct RClass *glsl;
  glsl = mrb_define_class(mrb, "Glsl", mrb->object_class);
  mrb_define_method(mrb, glsl, "initialize", mrb_glsl_init, MRB_ARGS_NONE());
  mrb_define_method(mrb, glsl, "hi", mrb_glsl_hi, MRB_ARGS_NONE());
  mrb_define_method(mrb, glsl, "render", mrb_glsl_render, MRB_ARGS_NONE());
  DONE;
}

void
mrb_mruby_glsl_gem_final(mrb_state *mrb)
{
}
