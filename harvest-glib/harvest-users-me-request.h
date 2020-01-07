#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_USERS_ME_REQUEST (harvest_users_me_request_get_type())
G_DECLARE_FINAL_TYPE(
	HarvestUsersMeRequest, harvest_users_me_request, HARVEST, HarvestUsersMe, GObject)

HarvestUsersMeRequest *harvest_users_me_request_new(void) G_GNUC_WARN_UNUSED_RESULT;

G_END_DECLS
