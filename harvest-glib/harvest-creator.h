#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_CREATOR (harvest_creator_get_type())
G_DECLARE_FINAL_TYPE(HarvestCreator, harvest_creator, HARVEST, CREATOR, GObject);

G_END_DECLS
