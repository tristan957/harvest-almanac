#pragma once


#include <glib-object.h>

#define HARVEST_TYPE_TASK_ASSIGNMENT (harvest_task_assignment_get_type())
G_DECLARE_FINAL_TYPE(
	HarvestTaskAssignment, harvest_task_assignment, HARVEST, TASK_ASSIGNMENT, GObject)
