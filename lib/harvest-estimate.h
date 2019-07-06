#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

#define HARVEST_TYPE_ESTIMATE (harvest_estimate_get_type())
G_DECLARE_FINAL_TYPE(HarvestEstimate, harvest_estimate, HARVEST, ESTIMATE, GObject)

typedef enum HarvestEstimateState
{
	HARVEST_ESTIMATE_STATE_DRAFT,
	HARVEST_ESTIMATE_STATE_SENT,
	HARVEST_ESTIMATE_STATE_ACCEPTED,
	HARVEST_ESTIMATE_STATE_DECLINED,
} HarvestEstimateState;
