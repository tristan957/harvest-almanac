#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#define HAL_TYPE_PROFILE (hal_profile_get_type())

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE(HalProfile, hal_profile, HAL, PROFILE, GtkStack)

HalProfile *hal_profile_new(void);

G_END_DECLS
