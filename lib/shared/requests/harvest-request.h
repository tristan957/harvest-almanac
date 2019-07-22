#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_REQUEST (harvest_request_get_type())
G_DECLARE_DERIVABLE_TYPE(HarvestRequest, harvest_reqeust, HARVEST, REQUEST, GObject)

struct _HarvestRequestClass
{
	GObjectClass parent_class;

	const char *(*serialize_params)(HarvestRequest *self);
};

const char *harvest_request_serialize_params(HarvestRequest *self);

G_END_DECLS
