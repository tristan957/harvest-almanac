#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define HAL_TYPE_APPLICATION (hal_application_get_type())
G_DECLARE_FINAL_TYPE(HalApplication, hal_application, HAL, APPLICATION, GtkApplication)

HalApplication *hal_application_new(const char *id);

G_END_DECLS
