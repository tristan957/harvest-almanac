#pragma once

#include <glib-object.h>

#include "shared/requests/harvest-request.h"

G_BEGIN_DECLS

#define HARVEST_TYPE_LATE_REQUEST (harvest_late_request_get_type())
G_DECLARE_FINAL_TYPE(
	HarvestLATERequest, harvest_late_request, HARVEST, LATE_REQUEST, HarvestRequest)

HarvestLATERequest *harvest_late_request_new(
	const char *first_prop_name, ...) G_GNUC_WARN_UNUSED_RESULT G_GNUC_NULL_TERMINATED;

G_END_DECLS
