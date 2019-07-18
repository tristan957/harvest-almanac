#pragma once


#include <glib-object.h>

#define HARVEST_TYPE_INVOICE_LINE_ITEM (harvest_invoice_line_item_get_type())
G_DECLARE_FINAL_TYPE(
	HarvestInvoiceLineItem, harvest_invoice_line_item, HARVEST, INVOICE_LINE_ITEM, GObject)

G_BEGIN_DECLS

G_END_DECLS
