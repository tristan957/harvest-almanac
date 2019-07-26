#pragma once

#include <curl/curl.h>
#include <glib-object.h>

#include "shared/requests/harvest-request.h"

G_BEGIN_DECLS

#define HARVEST_TYPE_API_CLIENT (harvest_api_client_get_type())
G_DECLARE_FINAL_TYPE(HarvestApiClient, harvest_api_client, HARVEST, API_CLIENT, GObject)

typedef enum HarvestApiVersion
{
	HARVEST_API_V2,
} HarvestApiVersion;

HarvestApiClient *harvest_api_client_new(const HarvestApiVersion version) G_GNUC_WARN_UNUSED_RESULT;
GObject *harvest_api_client_execute_request(
	HarvestApiClient *self, HarvestRequest *req, GError **err);

G_END_DECLS
