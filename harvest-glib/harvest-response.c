#include "config.h"

#define G_LOG_DOMAIN "HarvestResponse"

#include <glib-object.h>
#include <glib/gi18n-lib.h>

#include "harvest-http.h"
#include "harvest-response.h"

G_DEFINE_TYPE(HarvestResponse, harvest_response, G_TYPE_OBJECT)

enum HarvestResponseProps
{
	PROP_0,
	PROP_BODY,
	PROP_ERROR,
	PROP_STATUS_CODE,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_response_finalize(GObject *obj)
{
	HarvestResponse *self = HARVEST_RESPONSE(obj);

	if (self->body != NULL)
		g_object_unref(self->body);
	if (self->err != NULL)
		g_error_free(self->err);

	G_OBJECT_CLASS(harvest_response_parent_class)->finalize(obj);
}

static void
harvest_response_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestResponse *self = HARVEST_RESPONSE(obj);

	switch (prop_id) {
	case PROP_BODY:
		g_value_set_object(val, self->body);
		break;
	case PROP_ERROR:
		g_value_set_boxed(val, self->err);
		break;
	case PROP_STATUS_CODE:
		g_value_set_int(val, self->status_code);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_response_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestResponse *self = HARVEST_RESPONSE(obj);

	switch (prop_id) {
	case PROP_BODY:
		if (self->body != NULL)
			g_object_unref(self->body);
		self->body = g_value_dup_object(val);
		break;
	case PROP_ERROR:
		if (self->err != NULL)
			g_error_free(self->err);
		self->err = g_value_dup_boxed(val);
		break;
	case PROP_STATUS_CODE:
		self->status_code = g_value_get_int(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_response_class_init(HarvestResponseClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_response_finalize;
	obj_class->get_property = harvest_response_get_property;
	obj_class->set_property = harvest_response_set_property;

	obj_properties[PROP_BODY] = g_param_spec_object("body", _("Body"), _("Response body."),
		G_TYPE_OBJECT, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_ERROR]
		= g_param_spec_boxed("error", _("Error"), _("Why the request errored out."), G_TYPE_ERROR,
			G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_STATUS_CODE] = g_param_spec_int("status-code", _("Status Code"),
		_("Status code the response came back with"), HTTP_STATUS_OK,
		HTTP_STATUS_INTERNAL_SERVER_ERROR, HTTP_STATUS_OK,
		G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_response_init(G_GNUC_UNUSED HarvestResponse *self)
{}

HarvestResponse *
harvest_response_new(GObject *body, const HttpStatusCode code, GError *err)
{
	return g_object_new(
		HARVEST_TYPE_RESPONSE, "body", body, "status-code", code, "error", err, NULL);
}
