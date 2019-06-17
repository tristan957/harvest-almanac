#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <curl/curl.h>
#include <glib-object.h>

#define HARVEST_API_URL_V2 "https://api.harvestapp.com/v2"
#define HARVEST_TYPE_CLIENT (harvest_client_get_type())
G_DECLARE_FINAL_TYPE(HarvestClient, harvest_client, HARVEST, CLIENT, GObject)

typedef struct HarvestBuffer
{
	char *buf;
	size_t len;
} HarvestBuffer;

G_BEGIN_DECLS

HarvestClient *harvest_client_new();
GObject *harvest_client_get_request(
	HarvestClient *self, const GType type, const char *endpoint, GError **err);
GObject *harvest_client_post_request(
	HarvestClient *self, const GType type, const char *endpoint, GObject *data, GError **err);
size_t harvest_client_write_cb(char *ptr, size_t size, size_t nmemb, void *user_data);

G_END_DECLS
