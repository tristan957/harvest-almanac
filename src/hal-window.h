#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#define HAL_TYPE_WINDOW (hal_window_get_type())

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE(HalWindow, hal_window, HAL, WINDOW, GtkApplicationWindow)

HalWindow *hal_window_new(GApplication *app);

G_END_DECLS
