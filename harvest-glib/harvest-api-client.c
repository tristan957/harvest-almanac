#include "config.h"

#define G_LOG_DOMAIN "HarvestApiClient"

#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n-lib.h>
#include <json-glib/json-glib.h>
#include <libsoup/soup.h>

#include "harvest-api-client.h"
#include "harvest-common.h"
#include "harvest-http.h"
#include "harvest-request.h"
#include "harvest-response-metadata.h"
#include "harvest-response.h"

#define HARVEST_API_URL "https://api.harvestapp.com/v2"

static HarvestApiClient *instance;

struct _HarvestApiClient
{
	GObject parent_instance;

	SoupSession *session;
	char *server;
	char *access_token;
	char *account_id;
};

G_DEFINE_TYPE(HarvestApiClient, harvest_api_client, G_TYPE_OBJECT)

enum HarvestApiClientProps
{
	PROP_0,
	PROP_SESSION,
	PROP_SERVER,
	PROP_ACCESS_TOKEN,
	PROP_ACCOUNT_ID,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_api_client_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestApiClient *self = HARVEST_API_CLIENT(obj);

	switch (prop_id) {
	case PROP_SESSION:
		g_value_set_object(val, self->session);
		break;
	case PROP_SERVER:
		g_value_set_string(val, self->server);
		break;
	case PROP_ACCESS_TOKEN:
		g_value_set_string(val, self->access_token);
		break;
	case PROP_ACCOUNT_ID:
		g_value_set_string(val, self->account_id);
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
	case PROP_SESSION:
		if (self->session != NULL)
			g_object_unref(self->session);
		self->session = g_value_dup_object(val);
		break;
	case PROP_SERVER:
		g_free(self->server);
		self->server = g_value_dup_string(val);
		break;
	case PROP_ACCESS_TOKEN:
		g_free(self->access_token);
		self->access_token = g_strdup_printf("Bearer %s", g_value_get_string(val));
		break;
	case PROP_ACCOUNT_ID:
		g_free(self->account_id);
		self->account_id = g_value_dup_string(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
harvest_api_client_finalize(GObject *obj)
{
	HarvestApiClient *self = HARVEST_API_CLIENT(obj);

	g_free(self->server);
	g_free(self->access_token);
	g_free(self->account_id);

	G_OBJECT_CLASS(harvest_api_client_parent_class)->finalize(obj);
}

static void
harvest_api_client_class_init(HarvestApiClientClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_api_client_finalize;
	obj_class->get_property = harvest_api_client_get_property;
	obj_class->set_property = harvest_api_client_set_property;

	obj_properties[PROP_SESSION] = g_param_spec_object("session", _("Session"),
		_("SoupSession object to use to make requests."), SOUP_TYPE_SESSION,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_SERVER]
		= g_param_spec_string("server", _("Server"), _("Base URL for the server."), NULL,
			G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_ACCESS_TOKEN] = g_param_spec_string("access-token", _("Access Token"),
		_("Developer access token for the Harvest API."), NULL,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_ACCOUNT_ID]
		= g_param_spec_string("account-id", _("Account ID"), _("Harvest account ID to use."), NULL,
			G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_api_client_init(G_GNUC_UNUSED HarvestApiClient *self)
{}

HarvestApiClient *
harvest_api_client_new(SoupSession *session, const char *access_token, const char *account_id)
{
	g_return_val_if_fail(
		SOUP_IS_SESSION(session) && access_token != NULL && account_id != NULL, NULL);

	if (instance != NULL)
		g_object_unref(instance);

	if (instance == NULL) {
		instance = g_object_new(HARVEST_TYPE_API_CLIENT, "session", session, "server",
			HARVEST_API_URL, "access-token", access_token, "account-id", account_id, NULL);
	}

	return instance;
}

HarvestApiClient *
harvest_api_client_get_instance(void)
{
	return instance;
}

static void
harvest_api_client_destroy_request(HarvestRequest *req, gpointer user_data)
{
	g_object_unref(req);
	g_object_unref(HARVEST_RESPONSE(user_data));
}

static void
harvest_api_client_async_callback(
	G_GNUC_UNUSED SoupSession *Session, SoupMessage *msg, gpointer user_data)
{
	HarvestRequest *req				  = HARVEST_REQUEST(user_data);
	HarvestResponseMetadata *metadata = harvest_request_get_response_metadata(req);
	const GType body_type			  = harvest_response_metadata_get_body_type(metadata);

	GObject *body = NULL;
	GError *err	  = NULL;

	if (body_type != G_TYPE_NONE) {
		g_autoptr(SoupBuffer) buf = soup_message_body_flatten(msg->response_body);
		g_autoptr(GBytes) bytes	  = soup_buffer_get_as_bytes(buf);

		gsize size				= 0;
		gconstpointer body_data = g_bytes_get_data(bytes, &size);
		body					= json_gobject_from_data(body_type, body_data, size, &err);
	}

	HarvestResponse *response = harvest_response_new(body, msg->status_code, err);
	g_signal_connect_after(
		req, "completed", G_CALLBACK(harvest_api_client_destroy_request), response);
	g_signal_emit_by_name(req, "completed", response);
}

void
harvest_api_client_execute_request_async(HarvestApiClient *self, HarvestRequest *req)
{
	g_return_if_fail(HARVEST_IS_API_CLIENT(self) && HARVEST_IS_REQUEST(req));

	SoupMessage *msg	   = NULL;
	g_autoptr(GString) uri = g_string_new(self->server);
	g_string_append(uri, harvest_request_get_endpoint(req));
	gboolean response_has_body = harvest_request_get_data(req) != NULL;
	char *body				   = NULL;
	gsize len				   = 0;

	if (response_has_body) {
		body = json_gobject_to_data(harvest_request_get_data(req), &len);
		len	 = strlen(body);
	}

	switch (harvest_request_get_http_method(req)) {
	case HTTP_METHOD_GET:
		msg = soup_message_new("GET", uri->str);
		soup_message_set_request(
			msg, response_has_body ? "application/json" : NULL, SOUP_MEMORY_TAKE, body, len);
		break;
	case HTTP_METHOD_POST:
		break;
	case HTTP_METHOD_PATCH:
		break;
	case HTTP_METHOD_DELETE:
		break;
	default:
		g_return_if_reached();
	}

	soup_message_headers_append(msg->request_headers, "Authorization", self->access_token);
	soup_message_headers_append(msg->request_headers, "Harvest-Account-Id", self->account_id);

	soup_session_queue_message(self->session, msg, harvest_api_client_async_callback, req);
}
