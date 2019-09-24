#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#define HAL_TYPE_TIME_TRACKER (hal_time_tracker_get_type())
G_DECLARE_FINAL_TYPE(HalTimeTracker, hal_time_tracker, HAL, TIME_TRACKER, GtkBox)

G_BEGIN_DECLS

HalTimeTracker *hal_time_tracker_new(void) G_GNUC_WARN_UNUSED_RESULT;

G_END_DECLS
