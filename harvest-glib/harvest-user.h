#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_USER (harvest_user_get_type())
G_DECLARE_FINAL_TYPE(HarvestUser, harvest_user, HARVEST, USER, GObject);

HarvestUser *harvest_user_get_me();

G_END_DECLS
