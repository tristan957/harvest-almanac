#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#define HAL_TYPE_WINDOW (hal_window_get_type())
G_DECLARE_FINAL_TYPE(HalWindow, hal_window, HAL, WINDOW, GtkApplicationWindow)

G_BEGIN_DECLS

HalWindow *hal_window_new(GApplication *app) G_GNUC_WARN_UNUSED_RESULT;

G_END_DECLS
