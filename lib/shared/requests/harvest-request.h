#pragma once

#include <glib-object.h>

#include "shared/harvest-http.h"
#include "shared/responses/harvest-response-metadata.h"

G_BEGIN_DECLS

#define HARVEST_TYPE_REQUEST (harvest_request_get_type())
G_DECLARE_DERIVABLE_TYPE(HarvestRequest, harvest_request, HARVEST, REQUEST, GObject)

struct _HarvestRequestClass
{
	GObjectClass parent_class;
};

HttpMethod harvest_request_get_http_method(HarvestRequest *self) G_GNUC_CONST;
const char *harvest_request_get_endpoint(HarvestRequest *self) G_GNUC_CONST;
const char *harvest_request_get_query_params(
	HarvestRequest *self) G_GNUC_CONST G_GNUC_WARN_UNUSED_RESULT;
GObject *harvest_request_get_data(HarvestRequest *self) G_GNUC_CONST;
HarvestResponseMetadata *harvest_request_get_response_metadata(HarvestRequest *self) G_GNUC_CONST;

G_END_DECLS
