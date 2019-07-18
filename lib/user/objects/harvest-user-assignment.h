#pragma once


// https://help.getharvest.com/api-v2/projects-api/projects/user-assignments/

#include <glib-object.h>

#define HARVEST_TYPE_USER_ASSIGNMENT (harvest_invoice_get_type())
G_DECLARE_FINAL_TYPE(
	HarvestUserAssignment, harvest_user_assignment, HARVEST, USER_ASSIGNMENT, GObject)
