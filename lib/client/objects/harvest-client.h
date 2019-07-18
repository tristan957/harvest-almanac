#pragma once

#include <glib-object.h>

#define HARVEST_TYPE_CLIENT (harvest_client_get_type())
G_DECLARE_FINAL_TYPE(HarvestClient, harvest_client, HARVEST, CLIENT, GObject)
