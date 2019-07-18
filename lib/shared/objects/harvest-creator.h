#pragma once


#include <glib-object.h>

#define HARVEST_TYPE_CREATOR (harvest_creator_get_type())
G_DECLARE_FINAL_TYPE(HarvestCreator, harvest_creator, HARVEST, CREATOR, GObject);

G_BEGIN_DECLS

G_END_DECLS
