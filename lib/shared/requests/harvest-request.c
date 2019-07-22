#include <glib-object.h>

#include "harvest-request.h"

G_DEFINE_ABSTRACT_TYPE(HarvestRequest, harvest_request, G_TYPE_OBJECT)

const char *
harvest_request_default_serialize_params(G_GNUC_UNUSED HarvestRequest *self)
{
	return "";
}

static void
harvest_request_class_init(HarvestRequestClass *klass)
{
	klass->serialize_params = harvest_request_default_serialize_params;
}

static void
harvest_request_init(G_GNUC_UNUSED HarvestRequest *self)
{}

const char *
harvest_request_serialize_params(HarvestRequest *self)
{
	HarvestRequestClass *klass = HARVEST_REQUEST_GET_CLASS(self);

	return klass->serialize_params(self);
}
