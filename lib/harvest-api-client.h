#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <curl/curl.h>
#include <glib-object.h>

#define HARVEST_API_URL_V2 "https://api.harvestapp.com/v2"
#define HARVEST_TYPE_API_CLIENT (harvest_api_client_get_type())
G_DECLARE_FINAL_TYPE(HarvestApiClient, harvest_api_client, HARVEST, API_CLIENT, GObject)

typedef struct HarvestBuffer
{
	char *buf;
	size_t len;
} HarvestBuffer;

G_BEGIN_DECLS

HarvestApiClient *harvest_api_client_new() G_GNUC_WARN_UNUSED_RESULT;
GObject *harvest_client_get_request(
	HarvestApiClient *self, const GType type, const char *endpoint, GError **err);
GObject *harvest_client_post_request(
	HarvestApiClient *self, const GType type, const char *endpoint, GObject *data, GError **err);
size_t harvest_api_client_write_cb(char *ptr, size_t size, size_t nmemb, void *user_data);

G_END_DECLS
