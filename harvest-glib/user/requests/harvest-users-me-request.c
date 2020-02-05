#include "harvest-glib/config.h"

#include <glib-object.h>

#include "harvest-glib/http/harvest-http.h"
#include "harvest-glib/http/request/harvest-request.h"
#include "harvest-glib/http/response/harvest-response-metadata.h"
#include "harvest-glib/user/harvest-user.h"
#include "harvest-glib/user/requests/harvest-users-me-request.h"

struct _HarvestUsersMeRequest
{
	HarvestRequest parent_instance;
};

G_DEFINE_TYPE(HarvestUsersMeRequest, harvest_users_me_request, HARVEST_TYPE_REQUEST)

static void
harvest_users_me_request_class_init(G_GNUC_UNUSED HarvestUsersMeRequestClass *klass)
{}

static void
harvest_users_me_request_init(G_GNUC_UNUSED HarvestUsersMeRequest *self)
{}

HarvestUsersMeRequest *
harvest_users_me_request_new()
{
	g_autofree char *endpoint = g_strdup("/users/me");
	g_autoptr(HarvestResponseMetadata) response_metadata
		= harvest_response_metadata_new(HARVEST_TYPE_USER, HTTP_STATUS_OK);

	return g_object_new(HARVEST_TYPE_USERS_ME_REQUEST, "http-method", HTTP_METHOD_GET, "endpoint",
		endpoint, "response-metadata", response_metadata, NULL);
}
