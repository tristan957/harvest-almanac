#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>
#include <glib/gi18n.h>

#define HARVEST_TYPE_TIME_ENTRY (harvest_time_entry_get_type())
G_DECLARE_FINAL_TYPE(HarvestTimeEntry, harvest_time_entry, HARVEST, TIME_ENTRY, GObject)

G_BEGIN_DECLS

G_END_DECLS
