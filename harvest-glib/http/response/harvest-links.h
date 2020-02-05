#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_LINKS (harvest_links_get_type())
G_DECLARE_FINAL_TYPE(HarvestLinks, harvest_links, HARVEST, LINKS, GObject)

G_END_DECLS
