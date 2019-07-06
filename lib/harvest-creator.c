#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-creator.h"

struct _HarvestCreator
{
	GObject parent_instance;

	int id;
	char *name;
};

G_DEFINE_TYPE(HarvestCreator, harvest_creator, G_TYPE_OBJECT);

enum HarvestCreatorProps
{
	PROP_0,
	PROP_ID,
	PROP_NAME,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_creator_finalize(GObject *obj)
{
	HarvestCreator *self = HARVEST_CREATOR(obj);

	g_free(self->name);

	G_OBJECT_CLASS(harvest_creator_parent_class)->finalize(obj);
}

static void
harvest_creator_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestCreator *self = HARVEST_CREATOR(obj);

	switch (prop_id) {
	case PROP_ID:
		g_value_set_int(val, self->id);
		break;
	case PROP_NAME:
		g_value_set_string(val, self->name);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
harvest_creator_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestCreator *self = HARVEST_CREATOR(obj);

	switch (prop_id) {
	case PROP_ID:
		self->id = g_value_get_int(val);
		break;
	case PROP_NAME:
		g_free(self->name);
		self->name = g_value_dup_string(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
harvest_creator_class_init(HarvestCreatorClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_creator_finalize;
	obj_class->get_property = harvest_creator_get_property;
	obj_class->set_property = harvest_creator_set_property;

	obj_properties[PROP_ID]   = g_param_spec_int("id", _("ID"), _("Unique ID for the creator."), 0,
		  INT_MAX, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_NAME] = g_param_spec_string(
		"name", _("Name"), _("Name of the creator."), NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_creator_init(G_GNUC_UNUSED HarvestCreator *self)
{}
