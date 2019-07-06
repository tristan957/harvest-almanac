#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

#define HARVEST_TYPE_INVOICE_LINE_ITEM (harvest_invoice_line_item_get_type())
G_DECLARE_FINAL_TYPE(
	HarvestInvoiceLineItem, harvest_invoice_line_item, HARVEST, INVOICE_LINE_ITEM, GObject)

G_BEGIN_DECLS

G_END_DECLS
