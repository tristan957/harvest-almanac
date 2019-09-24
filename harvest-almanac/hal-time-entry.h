#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#define HAL_TYPE_TIME_ENTRY (hal_time_entry_get_type())
G_DECLARE_FINAL_TYPE(HalTimeEntry, hal_time_entry, HAL, TIME_ENTRY, GtkGrid)

G_BEGIN_DECLS

HalTimeEntry *hal_time_entry_new(void) G_GNUC_WARN_UNUSED_RESULT;
gboolean hal_time_entry_is_running(HalTimeEntry *self);
void hal_time_entry_stop(HalTimeEntry *self);

G_END_DECLS
