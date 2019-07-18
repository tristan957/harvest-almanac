#pragma once

#include <glib-object.h>

#define HARVEST_TYPE_INVOICE_ITEM_CATEGORY (harvest_invoice_item_category_get_type())
G_DECLARE_FINAL_TYPE(HarvestInvoiceItemCategory, harvest_invoice_item_category, HARVEST,
	INVOICE_ITEM_CATEGORY, GObject)
