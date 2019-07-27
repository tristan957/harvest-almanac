#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-request.h"
#include "shared/harvest-http.h"
#include "shared/responses/harvest-response.h"

typedef struct _HarvestRequestPrivate
{
	HttpMethod http_method;
	char *endpoint;
	char *query_params;
	GObject *data;
	HttpStatusCode expected_status;
	HarvestResponse *response;
} HarvestRequestPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(HarvestRequest, harvest_request, G_TYPE_OBJECT)

enum HarvestRequestProps
{
	PROP_0,
	PROP_HTTP_METHOD,
	PROP_ENDPOINT,
	PROP_QUERY_PARAMS,
	PROP_DATA,
	PROP_EXPECTED_STATUS,
	PROP_RESPONSE,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_request_finalize(GObject *obj)
{
	HarvestRequest *self		= HARVEST_REQUEST(obj);
	HarvestRequestPrivate *priv = harvest_request_get_instance_private(self);

	g_free(priv->endpoint);
	g_free(priv->query_params);
	if (priv->data != NULL)
		g_object_unref(priv->data);
	if (priv->response != NULL)
		g_object_unref(priv->response);

	G_OBJECT_CLASS(harvest_request_parent_class)->finalize(obj);
}

static void
harvest_request_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestRequest *self		= HARVEST_REQUEST(obj);
	HarvestRequestPrivate *priv = harvest_request_get_instance_private(self);

	switch (prop_id) {
	case PROP_HTTP_METHOD:
		g_value_set_int(val, priv->http_method);
		break;
	case PROP_ENDPOINT:
		g_value_set_string(val, priv->endpoint);
		break;
	case PROP_QUERY_PARAMS:
		g_value_set_string(val, priv->query_params);
		break;
	case PROP_DATA:
		g_value_set_object(val, priv->data);
		break;
	case PROP_EXPECTED_STATUS:
		g_value_set_int(val, priv->expected_status);
		break;
	case PROP_RESPONSE:
		g_value_set_object(val, priv->response);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_request_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestRequest *self		= HARVEST_REQUEST(obj);
	HarvestRequestPrivate *priv = harvest_request_get_instance_private(self);

	switch (prop_id) {
	case PROP_HTTP_METHOD:
		priv->http_method = g_value_get_int(val);
		break;
	case PROP_ENDPOINT:
		g_free(priv->endpoint);
		priv->endpoint = g_value_dup_string(val);
		break;
	case PROP_QUERY_PARAMS:
		g_free(priv->query_params);
		priv->query_params = g_value_dup_string(val);
		break;
	case PROP_DATA:
		if (priv->data != NULL)
			g_object_unref(priv->data);
		priv->data = g_value_dup_object(val);
		break;
	case PROP_EXPECTED_STATUS:
		priv->expected_status = g_value_get_int(val);
		break;
	case PROP_RESPONSE:
		if (priv->response != NULL)
			g_object_unref(priv->response);
		priv->response = g_value_dup_object(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_request_class_init(HarvestRequestClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_request_finalize;
	obj_class->get_property = harvest_request_get_property;
	obj_class->set_property = harvest_request_set_property;

	obj_properties[PROP_HTTP_METHOD]  = g_param_spec_int("http-method", _("HTTP Method"),
		 _("The HTTP method by which to send the request."), HTTP_METHOD_GET, HTTP_METHOD_DELETE,
		 HTTP_METHOD_GET, G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE);
	obj_properties[PROP_ENDPOINT]	 = g_param_spec_string("endpoint", _("Endpoint"),
		_("The server endpoint to send the request to."), NULL,
		G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE);
	obj_properties[PROP_QUERY_PARAMS] = g_param_spec_string("query-params", _("Query Params"),
		_("The query params to send the request with."), NULL,
		G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE);
	obj_properties[PROP_DATA]
		= g_param_spec_object("data", _("Data"), _("The data to send in the body of the request."),
			G_TYPE_OBJECT, G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE);
	obj_properties[PROP_EXPECTED_STATUS] = g_param_spec_int("expected-status", _("Expected Status"),
		_("The expected status code the response should come back with."), HTTP_STATUS_OK,
		HTTP_STATUS_GATEWAY_TIMEOUT, HTTP_STATUS_OK,
		G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE);
	obj_properties[PROP_RESPONSE]		 = g_param_spec_object("response", _("Response"),
		   _("An object containing meta information of the response."), HARVEST_TYPE_RESPONSE,
		   G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_request_init(G_GNUC_UNUSED HarvestRequest *self)
{}

HttpMethod
harvest_request_get_http_method(HarvestRequest *self)
{
	HarvestRequestPrivate *priv = harvest_request_get_instance_private(self);

	return priv->http_method;
}

const char *
harvest_request_get_endpoint(HarvestRequest *self)
{
	HarvestRequestPrivate *priv = harvest_request_get_instance_private(self);

	return priv->endpoint;
}

const char *
harvest_request_get_query_params(HarvestRequest *self)
{
	HarvestRequestPrivate *priv = harvest_request_get_instance_private(self);

	return priv->query_params;
}

GObject *
harvest_request_get_data(HarvestRequest *self)
{
	HarvestRequestPrivate *priv = harvest_request_get_instance_private(self);

	return priv->data;
}

HttpStatusCode
harvest_request_get_expected_status(HarvestRequest *self)
{
	HarvestRequestPrivate *priv = harvest_request_get_instance_private(self);

	return priv->expected_status;
}

HarvestResponse *
harvest_request_get_response(HarvestRequest *self)
{
	HarvestRequestPrivate *priv = harvest_request_get_instance_private(self);

	return priv->response;
}
