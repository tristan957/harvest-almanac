#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-project.h"

#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-project.h"

struct _HarvestProject
{
	GObject parent_instance;
};

G_DEFINE_TYPE(HarvestProject, harvest_project, G_TYPE_OBJECT)

enum HarvestProjectProps
{
	PROP_0,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_project_class_init(HarvestProjectClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_project_init(G_GNUC_UNUSED HarvestProject *self)
{}
