#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

#include "harvest-http.h"
#include "harvest-response-metadata.h"
#include "harvest-response.h"

G_BEGIN_DECLS

#define HARVEST_TYPE_REQUEST (harvest_request_get_type())
G_DECLARE_DERIVABLE_TYPE(HarvestRequest, harvest_request, HARVEST, REQUEST, GObject)

struct _HarvestRequestClass
{
	GObjectClass parent_class;
};

typedef void(HarvestQueuedCallback)(HarvestRequest *request, gpointer user_data);
typedef void(HarvestCompletedCallback)(
	HarvestRequest *request, HarvestResponse *response, gpointer user_data);

HttpMethod harvest_request_get_http_method(HarvestRequest *self) G_GNUC_CONST;
const char *harvest_request_get_endpoint(HarvestRequest *self) G_GNUC_CONST;
const char *harvest_request_get_query_params(
	HarvestRequest *self) G_GNUC_CONST G_GNUC_WARN_UNUSED_RESULT;
GObject *harvest_request_get_data(HarvestRequest *self) G_GNUC_CONST;
HarvestResponseMetadata *harvest_request_get_response_metadata(HarvestRequest *self) G_GNUC_CONST;

G_END_DECLS
