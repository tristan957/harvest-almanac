#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include <glib-object.h>
#include <glib/gi18n.h>
#include <json-glib/json-glib.h>
#include <libsoup/soup.h>

#include "harvest-api-client.h"
#include "harvest-error.h"
#include "shared/harvest-http.h"
#include "shared/requests/harvest-request.h"
#include "shared/responses/harvest-response-metadata.h"

#define HARVEST_API_URL_V2 "https://api.harvestapp.com/v2"

// TODO: Add access token and account id to struct for apply Authorization and Harvest-Account-Id
// headers
struct _HarvestApiClient
{
	GObject parent_instance;

	SoupSession *session;
	const char *server;
	char *application_name;
	char *contact_info;
};

G_DEFINE_TYPE(HarvestApiClient, harvest_api_client, G_TYPE_OBJECT)

enum HarvestApiClientProps
{
	PROP_0,
	PROP_SERVER,
	PROP_APPLICATION_NAME,
	PROP_CONTACT_INFO,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_api_client_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestApiClient *self = HARVEST_API_CLIENT(obj);

	switch (prop_id) {
	case PROP_SERVER:
		g_value_set_string(val, self->server);
		break;
	case PROP_APPLICATION_NAME:
		g_value_set_string(val, self->application_name);
		break;
	case PROP_CONTACT_INFO:
		g_value_set_string(val, self->contact_info);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
harvest_api_client_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestApiClient *self = HARVEST_API_CLIENT(obj);

	switch (prop_id) {
	case PROP_SERVER:
		self->server = g_value_get_string(val);
		break;
	case PROP_APPLICATION_NAME:
		g_free(self->application_name);
		self->application_name = g_value_dup_string(val);
		break;
	case PROP_CONTACT_INFO:
		g_free(self->contact_info);
		self->contact_info = g_value_dup_string(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
harvest_api_client_constructed(GObject *obj)
{
	HarvestApiClient *self = HARVEST_API_CLIENT(obj);

	g_autoptr(GString) user_agent = g_string_new(NULL);
	g_string_append_printf(user_agent, "%s (%s)", self->application_name, self->contact_info);
	self->session
		= soup_session_new_with_options(SOUP_SESSION_MAX_CONNS_PER_HOST, 4, SOUP_SESSION_USER_AGENT,
			user_agent->str, SOUP_SESSION_ADD_FEATURE_BY_TYPE, SOUP_TYPE_CONTENT_SNIFFER, NULL);

	SoupLoggerLogLevel log_level = SOUP_LOGGER_LOG_NONE;
#ifdef HARVEST_GLIB_DEBUG
#	ifdef HARVEST_GLIB_LIBSOUP_LOG_BODY
	log_level = SOUP_LOGGER_LOG_BODY;
#	elif HARVEST_GLIB_LIBSOUP_LOG_HEADERS
	log_level = SOUP_LOGGER_LOG_HEADERS;
#	elif HARVEST_GLIB_LIBSOUP_LOG_MINIMAL
	log_level = SOUP_LOGGER_LOG_MINIMAL;
#	endif
#endif
	g_autoptr(SoupLogger) logger = soup_logger_new(log_level, -1);
	soup_session_add_feature(self->session, SOUP_SESSION_FEATURE(logger));
}

static void
harvest_api_client_finalize(GObject *obj)
{
	HarvestApiClient *self = HARVEST_API_CLIENT(obj);

	g_free(self->application_name);
	g_free(self->contact_info);

	G_OBJECT_CLASS(harvest_api_client_parent_class)->finalize(obj);
}

static void
harvest_api_client_class_init(HarvestApiClientClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->constructed  = harvest_api_client_constructed;
	obj_class->finalize		= harvest_api_client_finalize;
	obj_class->get_property = harvest_api_client_get_property;
	obj_class->set_property = harvest_api_client_set_property;

	obj_properties[PROP_SERVER] = g_param_spec_string(
		"server", _("Server"), _("Location of the server"), NULL, G_PARAM_READWRITE);
	obj_properties[PROP_APPLICATION_NAME] = g_param_spec_string("application-name",
		_("Application Name"), _("Application name to use in the user agent string."), NULL,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	obj_properties[PROP_CONTACT_INFO]	 = g_param_spec_string("contact-url", _("Contact URL"),
		_("URL or email where Harvest can contact you in the case of misuse of their API."), NULL,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_api_client_init(G_GNUC_UNUSED HarvestApiClient *self)
{}

HarvestApiClient *
harvest_api_client_new(
	const HarvestApiVersion version, const char *application_name, const char *contact_info)
{
	g_return_val_if_fail(application_name != NULL && contact_info != NULL, NULL);

	switch (version) {
	case HARVEST_API_V2:
		return g_object_new(HARVEST_TYPE_API_CLIENT, "server", HARVEST_API_URL_V2,
			"application-name", application_name, "contact-info", contact_info, NULL);
	default:
		g_return_val_if_reached(NULL);
	}

	return NULL;
}

static GObject *
harvest_api_client_get_request(HarvestApiClient *self, HarvestRequest *req, GError **err)
{
	return NULL;
}

static GObject *
harvest_api_client_post_request(HarvestApiClient *self, HarvestRequest *req, GError **err)
{
	return NULL;
}

GObject *
harvest_api_client_execute_request(HarvestApiClient *self, HarvestRequest *req, GError **err)
{
	g_return_val_if_fail(HARVEST_IS_API_CLIENT(self) && HARVEST_IS_REQUEST(req), NULL);
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	switch (harvest_request_get_http_method(req)) {
	case HTTP_METHOD_GET:
		return harvest_api_client_get_request(self, req, err);
	case HTTP_METHOD_POST:
		return harvest_api_client_post_request(self, req, err);
	case HTTP_METHOD_PATCH:
		return NULL;
	case HTTP_METHOD_PUT:
		return NULL;
	case HTTP_METHOD_DELETE:
		return NULL;
	default:
		g_warn_if_reached();
	}

	return NULL;
}
