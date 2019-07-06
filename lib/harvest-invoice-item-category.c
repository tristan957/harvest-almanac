#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-invoice-item-category.h"

struct _HarvestInvoiceItemCategory
{
	GObject parent_instance;
};

G_DEFINE_TYPE(HarvestInvoiceItemCategory, harvest_invoice_item_category, G_TYPE_OBJECT)

enum HarvestInvoiceItemCategoryProps
{
	PROP_0,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_invoice_item_category_class_init(HarvestInvoiceItemCategoryClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_invoice_item_category_init(G_GNUC_UNUSED HarvestInvoiceItemCategory *self)
{}
