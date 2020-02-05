#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_TASK_ASSIGNMENT (harvest_task_assignment_get_type())
G_DECLARE_FINAL_TYPE(
	HarvestTaskAssignment, harvest_task_assignment, HARVEST, TASK_ASSIGNMENT, GObject)

G_END_DECLS
