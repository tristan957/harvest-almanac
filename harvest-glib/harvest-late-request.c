#include "config.h"
#define G_LOG_DOMAIN "HarvestLATERequest"

#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n-lib.h>

#include "harvest-http.h"
#include "harvest-late-request.h"
#include "harvest-response-metadata.h"

struct _HarvestLATERequest
{
	HarvestRequest parent_instance;

	int user_id;
	int client_id;
	int project_id;
	gboolean is_billed : 1;
	gboolean is_running : 1;
	GDateTime *updated_since;
	GDateTime *from;
	GDateTime *to;
	unsigned int page;
	unsigned int per_page;
};

typedef struct _HarvestLATERequestPrivate
{
	gboolean user_id : 1;
	gboolean client_id : 1;
	gboolean project_id : 1;
	gboolean is_billed : 1;
	gboolean is_running : 1;
	gboolean updated_since : 1;
	gboolean from : 1;
	gboolean to : 1;
	gboolean page : 1;
	gboolean per_page : 1;
} HarvestLATERequestPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HarvestLATERequest, harvest_late_request, HARVEST_TYPE_REQUEST)

enum HarvestLATERequest
{
	PROP_0,
	PROP_USER_ID,
	PROP_CLIENT_ID,
	PROP_PROJECT_ID,
	PROPIS_BILLED,
	PROP_IS_RUNNING,
	PROP_UPDATED_SINCE,
	PROP_FROM,
	PROP_TO,
	PROP_PAGE,
	PROP_PER_PAGE,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_late_request_finalize(GObject *obj)
{
	HarvestLATERequest *self = HARVEST_LATE_REQUEST(obj);

	if (self->from != NULL)
		g_date_time_unref(self->from);
	if (self->to != NULL)
		g_date_time_unref(self->to);
	if (self->updated_since != NULL)
		g_date_time_unref(self->updated_since);

	G_OBJECT_CLASS(harvest_late_request_parent_class)->finalize(obj);
}

G_GNUC_CONST G_GNUC_WARN_UNUSED_RESULT static const char *
harvest_late_request_serialize_params(HarvestLATERequest *self)
{
	HarvestLATERequestPrivate *priv = harvest_late_request_get_instance_private(self);

	g_autoptr(GString) string = g_string_new(NULL);

	if (priv->user_id)
		g_string_append_printf(string, "&user_id=%d", self->user_id);
	if (priv->client_id)
		g_string_append_printf(string, "&client_id=%d", self->client_id);

	return g_strdup_printf(string->str);
}

static void
harvest_late_request_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestLATERequest *self = HARVEST_LATE_REQUEST(obj);

	switch (prop_id) {
	case PROP_USER_ID:
		g_value_set_int(val, self->user_id);
		break;
	case PROP_CLIENT_ID:
		g_value_set_int(val, self->client_id);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_late_request_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestLATERequest *self		= HARVEST_LATE_REQUEST(obj);
	HarvestLATERequestPrivate *priv = harvest_late_request_get_instance_private(self);

	switch (prop_id) {
	case PROP_USER_ID:
		priv->user_id = TRUE;
		self->user_id = g_value_get_int(val);
		break;
	case PROP_CLIENT_ID:
		priv->client_id = TRUE;
		self->client_id = g_value_get_int(val);
		break;
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

	obj_properties[PROP_USER_ID]   = g_param_spec_int("user_id", _("User ID"), _(""), 0, INT_MAX, 0,
		  G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_CLIENT_ID] = g_param_spec_int("client_id", _("Client ID"), _(""), 0,
		INT_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, 3, obj_properties);
}

static void
harvest_late_request_init(G_GNUC_UNUSED HarvestLATERequest *self)
{}

HarvestLATERequest *
harvest_late_request_new(const char *first_prop_name, ...)
{
	va_list var_args;

	// Short-circuit for calls supplying no properties
	if (first_prop_name == NULL) {
		return HARVEST_LATE_REQUEST(
			g_object_new_with_properties(HARVEST_TYPE_LATE_REQUEST, 0, NULL, NULL));
	}

	va_start(var_args, first_prop_name);
	HarvestLATERequest *self = HARVEST_LATE_REQUEST(
		g_object_new_valist(HARVEST_TYPE_LATE_REQUEST, first_prop_name, var_args));
	va_end(var_args);

	g_autoptr(GString) endpoint = g_string_new("/time_entries");
	g_autoptr(HarvestResponseMetadata) response_metadata
		= harvest_response_metadata_new(G_TYPE_NONE, HTTP_STATUS_OK);
	g_object_set(self, "http-method", HTTP_METHOD_GET, "endpoint", endpoint->str, "query-params",
		harvest_late_request_serialize_params(self), "response-metadata", response_metadata, NULL);

	return self;
}
