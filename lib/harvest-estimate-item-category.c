#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-estimate-item-category.h"

struct _HarvestEstimateItemCategory
{
	int id;
	char *name;
	char *created_at;
	char *updated_at;
};

G_DEFINE_TYPE(HarvestEstimateItemCategory, harvest_estimate_item_category, G_TYPE_OBJECT)

enum HarvestEstimateItemCategoryProps
{
	PROP_0,
	PROP_ID,
	PROP_NAME,
	PROP_CREATED_AT,
	PROP_UPDATED_AT,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_estimate_item_category_finalize(GObject *obj)
{
	HarvestEstimateItemCategory *self = HARVEST_ESTIMATE_ITEM_CATEGORY(obj);

	g_free(self->name);
	g_free(self->created_at);
	g_free(self->updated_at);

	G_OBJECT_CLASS(harvest_estimate_item_category_parent_class)->finalize(obj);
}

static void
harvest_estimate_item_category_get_property(
	GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestEstimateItemCategory *self = HARVEST_ESTIMATE_ITEM_CATEGORY(obj);

	switch (prop_id) {
	case PROP_ID:
		g_value_set_int(val, self->id);
		break;
	case PROP_NAME:
		g_value_set_string(val, self->name);
		break;
	case PROP_CREATED_AT:
		g_value_set_string(val, self->created_at);
		break;
	case PROP_UPDATED_AT:
		g_value_set_string(val, self->updated_at);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
harvest_estimate_item_category_set_property(
	GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestEstimateItemCategory *self = HARVEST_ESTIMATE_ITEM_CATEGORY(obj);

	switch (prop_id) {
	case PROP_ID:
		self->id = g_value_get_int(val);
		break;
	case PROP_NAME:
		g_free(self->name);
		self->name = g_value_dup_string(val);
		break;
	case PROP_CREATED_AT:
		g_free(self->created_at);
		self->created_at = g_value_dup_string(val);
		break;
	case PROP_UPDATED_AT:
		g_free(self->updated_at);
		self->updated_at = g_value_dup_string(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
harvest_estimate_item_category_class_init(HarvestEstimateItemCategoryClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_estimate_item_category_finalize;
	obj_class->get_property = harvest_estimate_item_category_get_property;
	obj_class->set_property = harvest_estimate_item_category_set_property;

	obj_properties[PROP_ID] =
		g_param_spec_int("id", _("ID"), _("Unique ID for the estimate item category."), 0, INT_MAX,
			0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_NAME]		= g_param_spec_string("name", _("Name"),
		  _("The name of the estimate item category."), NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_CREATED_AT] = g_param_spec_string("created_at", _("Created At"),
		_("Date and time the estimate item category was created."), NULL,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_UPDATED_AT] = g_param_spec_string("updated_at", _("Updated At"),
		_("Date and time the estimate item category was last updated."), NULL,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_estimate_item_category_init(G_GNUC_UNUSED HarvestEstimateItemCategory *self)
{}
