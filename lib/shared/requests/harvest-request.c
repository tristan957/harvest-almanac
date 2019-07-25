#include <glib-object.h>

#include "harvest-request.h"
#include "shared/harvest-http.h"

typedef struct _HarvestRequestPrivate
{
	HttpMethod method;
	char *endpoint;
	char *params;
	HttpStatusCode status;
	GType response_type;
} HarvestRequestPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(HarvestRequest, harvest_request, G_TYPE_OBJECT)

G_GNUC_CONST static inline const char *
harvest_request_default_serialize_params(G_GNUC_UNUSED HarvestRequest *self)
{
	return "";
}

G_GNUC_CONST static inline GType
harvest_request_default_get_response_type(void)
{
	return G_TYPE_NONE;
}

static void
harvest_request_class_init(HarvestRequestClass *klass)
{
	klass->serialize_params  = harvest_request_default_serialize_params;
	klass->get_response_type = harvest_request_default_get_response_type;
}

static void
harvest_request_init(G_GNUC_UNUSED HarvestRequest *self)
{}

HttpMethod
harvest_request_get_http_method(HarvestRequest *self)
{
	HarvestRequestClass *klass = HARVEST_REQUEST_GET_CLASS(self);

	g_return_val_if_fail(klass->get_http_method != NULL, HTTP_METHOD_UNKNOWN);

	return klass->get_http_method();
}

const char *
harvest_request_get_endpoint(HarvestRequest *self)
{
	HarvestRequestClass *klass = HARVEST_REQUEST_GET_CLASS(self);

	g_return_val_if_fail(klass->get_endpoint != NULL, NULL);

	return klass->get_endpoint(self);
}

const char *
harvest_request_serialize_params(HarvestRequest *self)
{
	HarvestRequestClass *klass = HARVEST_REQUEST_GET_CLASS(self);

	return klass->serialize_params(self);
}

gboolean
harvest_request_is_acceptable(HarvestRequest *self, const HttpStatusCode status)
{
	HarvestRequestClass *klass = HARVEST_REQUEST_GET_CLASS(self);

	g_return_val_if_fail(klass->status_code_valid != NULL, FALSE);

	return klass->status_code_valid(status);
}

GType
harvest_request_get_return_type(HarvestRequest *self)
{
	HarvestRequestClass *klass = HARVEST_REQUEST_GET_CLASS(self);

	return klass->get_response_type();
}
