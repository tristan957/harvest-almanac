#pragma once

#include <curl/curl.h>
#include <glib-object.h>

#include "shared/requests/harvest-request.h"

G_BEGIN_DECLS

#define HARVEST_API_URL_V2 "https://api.harvestapp.com/v2"
#define HARVEST_TYPE_API_CLIENT (harvest_api_client_get_type())
G_DECLARE_FINAL_TYPE(HarvestApiClient, harvest_api_client, HARVEST, API_CLIENT, GObject)

typedef struct HarvestBuffer
{
	char *buf;
	size_t len;
} HarvestBuffer;

HarvestApiClient *harvest_api_client_new() G_GNUC_WARN_UNUSED_RESULT;
GObject *harvest_api_client_execute_request(HarvestApiClient *self, HarvestRequest *request);

G_END_DECLS
