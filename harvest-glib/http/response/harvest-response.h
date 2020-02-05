#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

#include "harvest-glib/http/harvest-http.h"

G_BEGIN_DECLS

#define HARVEST_TYPE_RESPONSE (harvest_response_get_type())
G_DECLARE_FINAL_TYPE(HarvestResponse, harvest_response, HARVEST, RESPONSE, GObject)

struct _HarvestResponse
{
	GObject parent_instance;

	GValue *body;
	HttpStatusCode status_code;
	GError *err;
};

HarvestResponse *harvest_response_new(GValue *obj, const HttpStatusCode code, GError *err);

G_END_DECLS
