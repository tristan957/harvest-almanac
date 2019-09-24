#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_PAGEABLE (harvest_pageable_get_type())
G_DECLARE_INTERFACE(HarvestPageable, harvest_pageable, HARVEST, PAGEABLE, GObject)

struct _HarvestPageableInterface
{
	GTypeInterface parent_iface;
};

G_END_DECLS
