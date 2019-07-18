#pragma once

#include <glib-object.h>

#define HARVEST_TYPE_USER (harvest_user_get_type())
G_DECLARE_FINAL_TYPE(HarvestUser, harvest_user, HARVEST, USER, GObject);

G_BEGIN_DECLS

G_END_DECLS
