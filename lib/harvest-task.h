#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

#define HARVEST_TYPE_TASK (harvest_task_get_type())
G_DECLARE_FINAL_TYPE(HarvestTask, harvest_task, HARVEST, TASK, GObject)
