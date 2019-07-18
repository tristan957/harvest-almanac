#pragma once


#include <glib-object.h>

#define HARVEST_TYPE_TASK (harvest_task_get_type())
G_DECLARE_FINAL_TYPE(HarvestTask, harvest_task, HARVEST, TASK, GObject)
