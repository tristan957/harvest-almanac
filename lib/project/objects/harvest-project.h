#pragma once


#include <glib-object.h>

#define HARVEST_TYPE_PROJECT (harvest_project_get_type())
G_DECLARE_FINAL_TYPE(HarvestProject, harvest_project, HARVEST, PROJECT, GObject)
