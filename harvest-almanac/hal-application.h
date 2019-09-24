#pragma once

#include <gtk/gtk.h>

#define HAL_TYPE_APPLICATION (hal_application_get_type())
G_DECLARE_FINAL_TYPE(HalApplication, hal_application, HAL, APPLICATION, GtkApplication)

G_BEGIN_DECLS

HalApplication *hal_application_new(const char *id) G_GNUC_WARN_UNUSED_RESULT G_GNUC_CONST;

G_END_DECLS
