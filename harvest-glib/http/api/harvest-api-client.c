#include "harvest-glib/config.h"

#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n-lib.h>
#include <json-glib/json-glib.h>
#include <libsoup/soup.h>

#include "harvest-glib/common/harvest-common.h"
#include "harvest-glib/http/api/harvest-api-client.h"
#include "harvest-glib/http/harvest-http.h"
#include "harvest-glib/http/request/harvest-request.h"
#include "harvest-glib/http/response/harvest-response-metadata.h"
#include "harvest-glib/http/response/harvest-response.h"

#define HARVEST_API_URL "https://api.harvestapp.com/v2"

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
		if (self->session != NULL) {
			soup_session_abort(self->session);
			g_object_unref(self->session);
		}
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

	if (self->session != NULL) {
		soup_session_abort(self->session);
		g_object_unref(self->session);
	}
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

	return g_object_new(HARVEST_TYPE_API_CLIENT, "session", session, "server", HARVEST_API_URL,
		"access-token", access_token, "account-id", account_id, NULL);
}

static void
harvest_api_client_destroy_request(
	HarvestRequest *req, HarvestResponse *res, G_GNUC_UNUSED gpointer user_data)
{
	g_object_unref(req);
	g_object_unref(res);
}

static HarvestResponse *
create_response(HarvestRequest *req, SoupMessage *msg)
{
	HarvestResponseMetadata *metadata = harvest_request_get_response_metadata(req);
	const GType body_type			  = harvest_response_metadata_get_body_type(metadata);
	const HttpStatusCode expected	  = harvest_response_metadata_get_expected_status(metadata);

	GValue val	= G_VALUE_INIT;
	GError *err = NULL;

	if (msg->status_code != expected) {
		g_set_error(&err, HARVEST_API_CLIENT_ERROR, HARVEST_API_CLIENT_ERROR_UNEXPECTED_STATUS,
			"create_response: unexpected status code of %u, expected %u", expected,
			msg->status_code);
	}

	if (err == NULL && body_type != G_TYPE_NONE) {
		/*
		 * This probably looks absolutely stupid to the naked eye, but goto cannot be used
		 * effectively here due to the location in which the response object is created, and not
		 * wanting to repeat its creation at every error branch. With this do/while, we can use
		 * break to provide effective control flow on error.
		 */
		do {
			g_autoptr(SoupBuffer) buf = soup_message_body_flatten(msg->response_body);
			g_autoptr(GBytes) bytes	  = soup_buffer_get_as_bytes(buf);

			gsize size				= 0;
			gconstpointer body_data = g_bytes_get_data(bytes, &size);

			g_autoptr(JsonNode) node = json_from_string(body_data, &err);
			if (err != NULL) {
				break;
			}

			if (json_node_is_null(node)) {
				break;
			}

			g_value_init(&val, body_type);

			switch (body_type) {
			case G_TYPE_INT:
			case G_TYPE_INT64:
				g_value_set_int64(&val, json_node_get_int(node));
				break;
			case G_TYPE_FLOAT:
			case G_TYPE_DOUBLE:
				g_value_set_double(&val, json_node_get_double(node));
				break;
			case G_TYPE_STRING:
				g_value_set_string(&val, json_node_dup_string(node));
				break;
			case G_TYPE_BOOLEAN:
				g_value_set_boolean(&val, json_node_get_boolean(node));
				break;
			default:
				// Handle non-static GTypes
				if (body_type == JSON_TYPE_NODE) {
					g_value_set_object(&val, node);
				} else if (body_type == JSON_TYPE_ARRAY) {
					g_value_set_object(&val, json_node_dup_array(node));
				} else if (body_type == JSON_TYPE_OBJECT) {
					g_value_set_object(&val, json_node_dup_object(node));
				} else {
					GObject *data = json_gobject_deserialize(body_type, node);
					if (data == NULL) {
						g_set_error_literal(&err, HARVEST_API_CLIENT_ERROR,
							HARVEST_API_CLIENT_ERROR_UNABLE_TO_DESERIALIZE_OBJECT,
							"create_response: unable to deserialize response body into a GObject");

						break;
					}

					g_value_set_object(&val, data);
				}
			}
		} while (FALSE);
	}

	if (err != NULL) {
		g_value_unset(&val);
	}

	HarvestResponse *res = harvest_response_new(&val, msg->status_code, err);

	return res;
}

static void
harvest_api_client_async_callback(
	G_GNUC_UNUSED SoupSession *Session, SoupMessage *msg, gpointer user_data)
{
	HarvestRequest *req	 = HARVEST_REQUEST(user_data);
	HarvestResponse *res = create_response(req, msg);

	g_signal_connect_after(req, "completed", G_CALLBACK(harvest_api_client_destroy_request), res);
	g_signal_emit_by_name(req, "completed", res);
}

static SoupMessage *
create_message(HarvestApiClient *self, HarvestRequest *req)
{
	SoupMessage *msg	 = NULL;
	g_autofree char *uri = g_strdup_printf("%s%s", self->server, harvest_request_get_endpoint(req));
	gboolean response_has_body = harvest_request_get_data(req) != NULL;
	char *body				   = NULL;
	gsize len				   = 0;

	if (response_has_body) {
		body = json_gobject_to_data(harvest_request_get_data(req), &len);
		len	 = strlen(body);
	}

	switch (harvest_request_get_http_method(req)) {
	case HTTP_METHOD_GET:
		msg = soup_message_new("GET", uri);
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
		g_return_val_if_reached(NULL);
	}

	soup_message_headers_append(msg->request_headers, "Authorization", self->access_token);
	soup_message_headers_append(msg->request_headers, "Harvest-Account-Id", self->account_id);

	return msg;
}

HarvestResponse *
harvest_api_client_execute_request_sync(HarvestApiClient *self, HarvestRequest *req)
{
	g_return_val_if_fail(HARVEST_IS_REQUEST(req), NULL);

	g_autoptr(SoupMessage) msg = create_message(self, req);
	soup_session_send_message(self->session, msg);
	HarvestResponse *res = create_response(req, msg);
	g_signal_emit_by_name(req, "completed", res);

	return res;
}

void
harvest_api_client_execute_request_async(HarvestApiClient *self, HarvestRequest *req)
{
	g_return_if_fail(HARVEST_IS_REQUEST(req));

	SoupMessage *msg = create_message(self, req);

	soup_session_queue_message(self->session, msg, harvest_api_client_async_callback, req);
}

G_DEFINE_QUARK(harvest_api_client_error, harvest_api_client_error)
