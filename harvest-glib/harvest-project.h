#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_PROJECT (harvest_project_get_type())
G_DECLARE_FINAL_TYPE(HarvestProject, harvest_project, HARVEST, PROJECT, GObject)

G_END_DECLS
