#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define HAL_TYPE_TIME_ENTRY (hal_time_entry_get_type())
G_DECLARE_FINAL_TYPE(HalTimeEntry, hal_time_entry, HAL, TIME_ENTRY, GtkGrid)

HalTimeEntry *hal_time_entry_new(void);

G_END_DECLS
