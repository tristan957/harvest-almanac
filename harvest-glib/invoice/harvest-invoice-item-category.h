#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_INVOICE_ITEM_CATEGORY (harvest_invoice_item_category_get_type())
G_DECLARE_FINAL_TYPE(HarvestInvoiceItemCategory, harvest_invoice_item_category, HARVEST,
	INVOICE_ITEM_CATEGORY, GObject)

G_END_DECLS
