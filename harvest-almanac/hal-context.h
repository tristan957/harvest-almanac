#pragma once

#include <glib-object.h>
#include <harvest-glib/harvest.h>

G_BEGIN_DECLS

#define HAL_TYPE_CONTEXT (hal_context_get_type())
G_DECLARE_FINAL_TYPE(HalContext, hal_context, HAL, CONTEXT, GObject)

void hal_context_initialize();
const HalContext *hal_context_get() G_GNUC_CONST;
HarvestUser *hal_context_get_user() G_GNUC_CONST;
void hal_context_set_user(HarvestUser *user);

G_END_DECLS
