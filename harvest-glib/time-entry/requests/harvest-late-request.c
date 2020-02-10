#include "harvest-glib/config.h"

#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n-lib.h>

#include "harvest-glib/http/harvest-http.h"
#include "harvest-glib/http/response/harvest-response-metadata.h"
#include "harvest-glib/time-entry/requests/harvest-late-request.h"

struct _HarvestLATERequest
{
	GObject parent_instance;

	GVariant *user_id;	  // int
	GVariant *client_id;  // int
	GVariant *project_id; // int
	GVariant *is_billed;  // gboolean
	GVariant *is_running; // gboolean
	GDateTime *updated_since;
	GDateTime *from;
	GDateTime *to;
};

G_DEFINE_TYPE(HarvestLATERequest, harvest_late_request, HARVEST_TYPE_REQUEST)

enum HarvestLATERequest
{
	PROP_0,
	PROP_USER_ID,
	PROP_CLIENT_ID,
	PROP_PROJECT_ID,
	PROP_IS_BILLED,
	PROP_IS_RUNNING,
	PROP_UPDATED_SINCE,
	PROP_FROM,
	PROP_TO,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_late_request_finalize(GObject *obj)
{
	HarvestLATERequest *self = HARVEST_LATE_REQUEST(obj);

	if (self->user_id != NULL)
		g_variant_unref(self->user_id);
	if (self->client_id != NULL)
		g_variant_unref(self->client_id);
	if (self->project_id != NULL)
		g_variant_unref(self->project_id);
	if (self->is_billed != NULL)
		g_variant_unref(self->is_billed);
	if (self->is_running != NULL)
		g_variant_unref(self->is_running);
	if (self->from != NULL)
		g_date_time_unref(self->from);
	if (self->to != NULL)
		g_date_time_unref(self->to);
	if (self->updated_since != NULL)
		g_date_time_unref(self->updated_since);

	G_OBJECT_CLASS(harvest_late_request_parent_class)->finalize(obj);
}

static const char *G_GNUC_CONST G_GNUC_WARN_UNUSED_RESULT
harvest_late_request_serialize_params(HarvestLATERequest *self)
{
	g_autoptr(GString) string = g_string_new(NULL);

	if (self->user_id != NULL)
		g_string_append_printf(string, "&user_id=%d", g_variant_get_int32(self->user_id));
	if (self->client_id != NULL)
		g_string_append_printf(string, "&client_id=%d", g_variant_get_int32(self->client_id));

	return g_strdup(string->str);
}

static void
harvest_late_request_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestLATERequest *self = HARVEST_LATE_REQUEST(obj);

	switch (prop_id) {
	case PROP_USER_ID:
		g_value_set_variant(val, self->user_id);
		break;
	case PROP_CLIENT_ID:
		g_value_set_variant(val, self->client_id);
		break;
	case PROP_PROJECT_ID:
	case PROP_IS_BILLED:
	case PROP_IS_RUNNING:
	case PROP_UPDATED_SINCE:
	case PROP_FROM:
	case PROP_TO:
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_late_request_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestLATERequest *self = HARVEST_LATE_REQUEST(obj);

	switch (prop_id) {
	case PROP_USER_ID:
		if (self->user_id != NULL)
			g_variant_unref(self->user_id);
		self->user_id = g_value_dup_variant(val);
		break;
	case PROP_CLIENT_ID:
		if (self->client_id != NULL)
			g_variant_unref(self->client_id);
		self->client_id = g_value_dup_variant(val);
		break;
	case PROP_PROJECT_ID:
	case PROP_IS_BILLED:
	case PROP_IS_RUNNING:
	case PROP_UPDATED_SINCE:
	case PROP_FROM:
	case PROP_TO:
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_late_request_class_init(HarvestLATERequestClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_late_request_finalize;
	obj_class->get_property = harvest_late_request_get_property;
	obj_class->set_property = harvest_late_request_set_property;

	// TODO: Finish implemeting this
	obj_properties[PROP_USER_ID]   = g_param_spec_variant("user-id", _("User ID"), _(""),
		  G_VARIANT_TYPE_INT32, NULL, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_CLIENT_ID] = g_param_spec_variant("client-id", _("Client ID"), _(""),
		G_VARIANT_TYPE_INT32, NULL, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, 3, obj_properties);
}

static void
harvest_late_request_init(G_GNUC_UNUSED HarvestLATERequest *self)
{}

HarvestLATERequest *
harvest_late_request_new(const char *first_prop_name, ...)
{
	HarvestLATERequest *req = NULL;

	// Short-circuit for calls supplying no properties
	if (first_prop_name == NULL) {
		req = HARVEST_LATE_REQUEST(
			g_object_new_with_properties(HARVEST_TYPE_LATE_REQUEST, 0, NULL, NULL));
	} else {
		va_list var_args;
		va_start(var_args, first_prop_name);
		req = HARVEST_LATE_REQUEST(
			g_object_new_valist(HARVEST_TYPE_LATE_REQUEST, first_prop_name, var_args));
		va_end(var_args);
	}

	g_autoptr(HarvestResponseMetadata) response_metadata
		= harvest_response_metadata_new(G_TYPE_NONE, HTTP_STATUS_OK);

	g_object_set(req, "http-method", HTTP_METHOD_GET, "endpoint", "/time_entries",
		"response-metadata", response_metadata, "query-params",
		harvest_late_request_serialize_params(req), NULL);

	return req;
}
