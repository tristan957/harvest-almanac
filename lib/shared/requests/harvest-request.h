#pragma once

#include <glib-object.h>

#include "shared/harvest-http.h"

G_BEGIN_DECLS

#define HARVEST_TYPE_REQUEST (harvest_request_get_type())
G_DECLARE_DERIVABLE_TYPE(HarvestRequest, harvest_request, HARVEST, REQUEST, GObject)

struct _HarvestRequestClass
{
	GObjectClass parent_class;

	HttpMethod (*G_GNUC_CONST get_http_method)(void);
	const char *(*G_GNUC_CONST get_endpoint)(HarvestRequest *self);
	const char *(*G_GNUC_CONST G_GNUC_WARN_UNUSED_RESULT serialize_params)(HarvestRequest *self);
	gboolean (*G_GNUC_CONST status_code_valid)(const HttpStatusCode status);
	GType (*G_GNUC_CONST get_response_type)(void);
};

HttpMethod harvest_request_get_http_method(HarvestRequest *self) G_GNUC_CONST;
const char *harvest_request_get_endpoint(HarvestRequest *self) G_GNUC_CONST;
const char *harvest_request_serialize_params(
	HarvestRequest *self) G_GNUC_CONST G_GNUC_WARN_UNUSED_RESULT;
gboolean harvest_request_status_code_valid(
	HarvestRequest *self, const HttpStatusCode status) G_GNUC_CONST;
GType harvest_request_get_response_type(HarvestRequest *self) G_GNUC_CONST;

G_END_DECLS
