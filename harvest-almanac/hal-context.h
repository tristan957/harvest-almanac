#pragma once

#include <glib-object.h>
#include <harvest-glib/harvest.h>

G_BEGIN_DECLS

#define HAL_TYPE_CONTEXT (hal_context_get_type())
G_DECLARE_FINAL_TYPE(HalContext, hal_context, HAL, CONTEXT, GObject)

void hal_context_initialize();
void hal_context_set_user(HarvestUser *user);
HarvestUser *hal_context_get_user() G_GNUC_CONST;

G_END_DECLS
