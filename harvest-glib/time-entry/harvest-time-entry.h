#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_TIME_ENTRY (harvest_time_entry_get_type())
G_DECLARE_FINAL_TYPE(HarvestTimeEntry, harvest_time_entry, HARVEST, TIME_ENTRY, GObject)

G_END_DECLS
