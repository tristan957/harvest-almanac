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

HarvestApiClient *harvest_api_client_new(SoupSession *session, const char *access_token,
	const char *account_id) G_GNUC_WARN_UNUSED_RESULT;
HarvestApiClient *harvest_api_client_get_instance(void) G_GNUC_CONST;
HarvestResponse *harvest_api_client_execute_request_sync(
	HarvestApiClient *self, HarvestRequest *req);
void harvest_api_client_execute_request_async(HarvestApiClient *self, HarvestRequest *req);

G_END_DECLS
