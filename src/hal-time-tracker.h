#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#define HAL_TYPE_TIME_TRACKER (hal_time_tracker_get_type())

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE(HalTimeTracker, hal_time_tracker, HAL, TIME_TRACKER, GtkBox)

HalTimeTracker *hal_time_tracker_new(void);

G_END_DECLS
