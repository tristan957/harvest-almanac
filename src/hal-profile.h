#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#define HAL_TYPE_PROFILE (hal_profile_get_type())
G_DECLARE_FINAL_TYPE(HalProfile, hal_profile, HAL, PROFILE, GtkStack)

G_BEGIN_DECLS

HalProfile *hal_profile_new(void) G_GNUC_WARN_UNUSED_RESULT;

G_END_DECLS
