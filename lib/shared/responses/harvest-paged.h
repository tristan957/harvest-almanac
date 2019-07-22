#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_PAGED (harvest_paged_get_type())
G_DECLARE_DERIVABLE_TYPE(HarvestPaged, harvest_paged, HARVEST, PAGED, GObject)

struct _HarvestPagedClass
{
	GObjectClass parent_class;
};

G_END_DECLS
