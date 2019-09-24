#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_INVOICE (harvest_invoice_get_type())
G_DECLARE_FINAL_TYPE(HarvestInvoice, harvest_invoice, HARVEST, INVOICE, GObject)

typedef enum HarvestInvoiceState
{
	HARVEST_INVOICE_STATE_DRAFT,
	HARVEST_INVOICE_STATE_OPEN,
	HARVEST_INVOICE_STATE_PAID,
	HARVEST_INVOICE_STATE_CLOSED,
} HarvestInvoiceState;

typedef enum HarvestInvoicePaymentTerm
{
	HARVEST_INVOICE_PAYMENT_TERM_UPON_RECEIPT,
	HARVEST_INVOICE_PAYMENT_TERM_NET_15,
	HARVEST_INVOICE_PAYMENT_TERM_NET_30,
	HARVEST_INVOICE_PAYMENT_TERM_NET_45,
	HARVEST_INVOICE_PAYMENT_TERM_NET_60,
	HARVEST_INVOICE_PAYMENT_TERM_CUSTOM,
} HarvestInvoicePaymentTerm;

G_END_DECLS
