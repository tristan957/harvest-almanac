#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

#include "harvest-api-client.h"
#include "harvest-request.h"

G_BEGIN_DECLS

#define HARVEST_TYPE_USER (harvest_user_get_type())
G_DECLARE_FINAL_TYPE(HarvestUser, harvest_user, HARVEST, USER, GObject)

void harvest_user_get_me_async(HarvestCompletedCallback *callback, gpointer user_data);

G_END_DECLS
