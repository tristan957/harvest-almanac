#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

#define HARVEST_TYPE_ESTIMATE_ITEM_CATEGORY (harvest_estimate_item_category_get_type())
G_DECLARE_FINAL_TYPE(HarvestEstimateItemCategory, harvest_estimate_item_category, HARVEST,
	ESTIMATE_ITEM_CATEGORY, GObject)
