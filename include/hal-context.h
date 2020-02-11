#pragma once

#include <glib-object.h>
#include <harvest-glib/harvest.h>

G_BEGIN_DECLS

#define HAL_TYPE_CONTEXT (hal_context_get_type())
G_DECLARE_FINAL_TYPE(HalContext, hal_context, HAL, CONTEXT, GObject)

void hal_context_initialize(void);
void hal_context_set_user(HarvestUser *user);
HarvestUser *hal_context_get_user(void) G_GNUC_CONST;
void hal_context_set_company(HarvestCompany *company);
HarvestCompany *hal_context_get_company(void) G_GNUC_CONST;

G_END_DECLS
