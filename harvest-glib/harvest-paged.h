#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_PAGED (harvest_paged_get_type())
G_DECLARE_DERIVABLE_TYPE(HarvestPaged, harvest_paged, HARVEST, PAGED, GObject)

struct _HarvestPagedClass
{
	GObjectClass parent_class;
};

G_END_DECLS
