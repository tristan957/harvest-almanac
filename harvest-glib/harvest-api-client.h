#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>
#include <libsoup/soup.h>

#include "harvest-request.h"
#include "harvest-response.h"

G_BEGIN_DECLS

#define HARVEST_TYPE_API_CLIENT (harvest_api_client_get_type())
G_DECLARE_FINAL_TYPE(HarvestApiClient, harvest_api_client, HARVEST, API_CLIENT, GObject)

typedef gboolean (*HarvestAsyncCallback)(HarvestResponse *response);

typedef enum HarvestApiVersion
{
	HARVEST_API_V2 = 2,
} HarvestApiVersion;

HarvestApiClient *harvest_api_client_v2_new(SoupSession *session, const char *access_token,
	unsigned int account_id) G_GNUC_WARN_UNUSED_RESULT;
HarvestApiClient *harvest_api_client_get_instance(const HarvestApiVersion version) G_GNUC_CONST;
void harvest_api_client_execute_request_async(
	HarvestApiClient *self, HarvestRequest *req, HarvestAsyncCallback callback, gpointer user_data);

G_END_DECLS
