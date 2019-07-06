#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-task.h"

struct _HarvestTask
{
	GObject parent_instance;
};

G_DEFINE_TYPE(HarvestTask, harvest_task, G_TYPE_OBJECT)

enum HarvestTaskProps
{
	PROP_0,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_task_class_init(HarvestTaskClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_task_init(G_GNUC_UNUSED HarvestTask *self)
{}
