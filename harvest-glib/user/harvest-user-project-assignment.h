#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_USER_PROJECT_ASSIGNMENT (harvest_user_project_assignment_get_type())
G_DECLARE_FINAL_TYPE(HarvestUserProjectAssignment, harvest_user_project_assignment, HARVEST,
	USER_PROJECT_ASSIGNMENT, GObject)

G_END_DECLS
