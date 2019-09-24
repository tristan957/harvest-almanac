#include "config.h"
#define G_LOG_DOMAIN "HarvestEstimateLineItem"

#include <float.h>
#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n-lib.h>

#include "harvest-estimate-line-item.h"

struct _HarvestEstimateLineItem
{
	GObject parent_instance;

	int id;
	char *kind;
	char *description;
	int quantity;
	double unit_price;
	double amount;
	gboolean taxed : 1;
	gboolean taxed2 : 1;
};

G_DEFINE_TYPE(HarvestEstimateLineItem, harvest_estimate_line_item, G_TYPE_OBJECT)

enum HarvestEstimateLineItemProps
{
	PROP_0,
	PROP_ID,
	PROP_KIND,
	PROP_DESCRIPTION,
	PROP_QUANTITY,
	PROP_UNIT_PRICE,
	PROP_AMOUNT,
	PROP_TAXED,
	PROP_TAXED2,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_estimate_line_item_finalize(GObject *obj)
{
	HarvestEstimateLineItem *self = HARVEST_ESTIMATE_LINE_ITEM(obj);

	g_free(self->kind);
	g_free(self->description);

	G_OBJECT_CLASS(harvest_estimate_line_item_parent_class)->finalize(obj);
}

static void
harvest_estimate_line_item_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestEstimateLineItem *self = HARVEST_ESTIMATE_LINE_ITEM(obj);

	switch (prop_id) {
	case PROP_ID:
		g_value_set_int(val, self->id);
		break;
	case PROP_KIND:
		g_value_set_string(val, self->kind);
		break;
	case PROP_DESCRIPTION:
		g_value_set_string(val, self->description);
		break;
	case PROP_QUANTITY:
		g_value_set_int(val, self->quantity);
		break;
	case PROP_UNIT_PRICE:
		g_value_set_double(val, self->unit_price);
		break;
	case PROP_AMOUNT:
		g_value_set_double(val, self->amount);
		break;
	case PROP_TAXED:
		g_value_set_boolean(val, self->taxed);
		break;
	case PROP_TAXED2:
		g_value_set_boolean(val, self->taxed2);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_estimate_line_item_set_property(
	GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestEstimateLineItem *self = HARVEST_ESTIMATE_LINE_ITEM(obj);

	switch (prop_id) {
	case PROP_ID:
		self->id = g_value_get_int(val);
		break;
	case PROP_KIND:
		g_free(self->kind);
		self->kind = g_value_dup_string(val);
		break;
	case PROP_DESCRIPTION:
		g_free(self->description);
		self->description = g_value_dup_string(val);
		break;
	case PROP_QUANTITY:
		self->quantity = g_value_get_int(val);
		break;
	case PROP_UNIT_PRICE:
		self->unit_price = g_value_get_double(val);
		break;
	case PROP_AMOUNT:
		self->amount = g_value_get_double(val);
		break;
	case PROP_TAXED:
		self->taxed = g_value_get_boolean(val);
		break;
	case PROP_TAXED2:
		self->taxed2 = g_value_get_boolean(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_estimate_line_item_class_init(HarvestEstimateLineItemClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_estimate_line_item_finalize;
	obj_class->get_property = harvest_estimate_line_item_get_property;
	obj_class->set_property = harvest_estimate_line_item_set_property;

	obj_properties[PROP_ID] = g_param_spec_int("id", _("ID"), _("Unique ID for the line item."), 0,
		INT_MAX, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_KIND]
		= g_param_spec_string("kind", _("Kind"), _("The name of an estimate item category."), NULL,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_DESCRIPTION] = g_param_spec_string("description", _("Description"),
		_("Text description of the line item."), NULL,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_QUANTITY]
		= g_param_spec_int("quantity", _("Quantity"), _("The unit quantity of the item."), 0,
			INT_MAX, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_UNIT_PRICE]
		= g_param_spec_double("unit_price", _("Unit Price"), _("The individual price per unit."), 0,
			DBL_MAX, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_AMOUNT] = g_param_spec_double("amount", _("Amount"),
		_("The line item subtotal (quantity * unit_price)."), 0, DBL_MAX, 0,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_TAXED]  = g_param_spec_boolean("taxed", _("Taxed"),
		 _("Whether the estimate’s tax percentage applies to this line item."), FALSE,
		 G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_TAXED2] = g_param_spec_boolean("taxed2", _("Taxed2"),
		_("Whether the estimate’s tax2 percentage applies to this line item."), FALSE,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_estimate_line_item_init(G_GNUC_UNUSED HarvestEstimateLineItem *self)
{}
