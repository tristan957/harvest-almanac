#pragma once

#include <glib-object.h>

#include "shared/harvest-http.h"

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
HttpStatusCode harvest_request_get_expected_status(HarvestRequest *self) G_GNUC_CONST;
GType harvest_request_get_response_type(HarvestRequest *self) G_GNUC_CONST;

G_END_DECLS
