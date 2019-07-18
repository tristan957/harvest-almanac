#pragma once


#include <glib-object.h>

#define HARVEST_TYPE_ESTIMATE_LINE_ITEM (harvest_estimate_line_item_get_type())
G_DECLARE_FINAL_TYPE(
	HarvestEstimateLineItem, harvest_estimate_line_item, HARVEST, ESTIMATE_LINE_ITEM, GObject)
