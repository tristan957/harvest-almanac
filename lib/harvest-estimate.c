#include <float.h>
#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-client.h"
#include "harvest-creator.h"
#include "harvest-estimate-line-item.h"
#include "harvest-estimate.h"

struct _HarvestEstimate
{
	GObject parent_instance;

	int id;
	HarvestClient *client;
	GPtrArray *line_items; // HarvestEstimateLineItem
	HarvestCreator *creator;
	char *client_key;
	char *number;
	char *purchase_order;
	double amount;
	double tax;
	double tax_amount;
	double tax2;
	double tax2_amount;
	double discount;
	double discount_amount;
	char *subject;
	char *notes;
	char *currency;
	HarvestEstimateState state;
	GDateTime *issue_date;
	GDateTime *sent_at;
	GDateTime *accepted_at;
	GDateTime *declined_at;
	GDateTime *created_at;
	GDateTime *updated_at;
};

G_DEFINE_TYPE(HarvestEstimate, harvest_estimate, G_TYPE_OBJECT)

enum HarvestEstimateProps
{
	PROP_0,
	PROP_ID,
	PROP_CLIENT,
	PROP_LINE_ITEMS,
	PROP_CREATOR,
	PROP_CLIENT_KEY,
	PROP_NUMBER,
	PROP_PURCHASE_ORDER,
	PROP_AMOUNT,
	PROP_TAX,
	PROP_TAX_AMOUNT,
	PROP_TAX2,
	PROP_TAX2_AMOUNT,
	PROP_DISCOUNT,
	PROP_DISCOUNT_AMOUNT,
	PROP_SUBJECT,
	PROP_NOTES,
	PROP_CURRENCY,
	PROP_STATE,
	PROP_ISSUE_DATE,
	PROP_SENT_AT,
	PROP_ACCEPTED_AT,
	PROP_DECLINED_AT,
	PROP_CREATED_AT,
	PROP_UPDATED_AT,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_estimate_finalize(GObject *obj)
{
	HarvestEstimate *self = HARVEST_ESTIMATE(obj);

	if (self->client != NULL)
		g_object_unref(self->client);
	if (self->line_items != NULL) {
		/**
		 * Always remove the last one so that the array does not need to be resized. If we
		 * always remove the last one in conjunction with g_ptr_array_remove_index_fast, then
		 * the reverse order of the array should be preserved and we will be able to iterate
		 * linearly.
		 */
		for (int i = self->line_items->len - 1; i >= 0; i--) {
			HarvestEstimateLineItem *line_item =
				HARVEST_ESTIMATE_LINE_ITEM(g_ptr_array_remove_index_fast(self->line_items, i));
			if (line_item != NULL)
				g_object_unref(line_item);
		}
		g_ptr_array_unref(self->line_items);
	}
	if (self->creator != NULL)
		g_object_unref(self->creator);
	g_free(self->client_key);
	g_free(self->number);
	g_free(self->purchase_order);
	g_free(self->subject);
	g_free(self->notes);
	g_free(self->currency);
	if (self->issue_date != NULL)
		g_date_time_unref(self->issue_date);
	if (self->sent_at != NULL)
		g_date_time_unref(self->sent_at);
	if (self->accepted_at != NULL)
		g_date_time_unref(self->accepted_at);
	if (self->declined_at != NULL)
		g_date_time_unref(self->declined_at);
	if (self->created_at != NULL)
		g_date_time_unref(self->created_at);
	if (self->updated_at != NULL)
		g_date_time_unref(self->updated_at);

	G_OBJECT_CLASS(harvest_estimate_parent_class)->finalize(obj);
}

static void
harvest_estimate_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestEstimate *self = HARVEST_ESTIMATE(obj);

	switch (prop_id) {
	case PROP_ID:
		g_value_set_int(val, self->id);
		break;
	case PROP_CLIENT:
		g_value_set_object(val, self->client);
		break;
	case PROP_LINE_ITEMS:
		g_value_set_boxed(val, self->line_items);
		break;
	case PROP_CREATOR:
		g_value_set_object(val, self->creator);
		break;
	case PROP_CLIENT_KEY:
		g_value_set_string(val, self->client_key);
		break;
	case PROP_NUMBER:
		g_value_set_string(val, self->number);
		break;
	case PROP_PURCHASE_ORDER:
		g_value_set_string(val, self->purchase_order);
		break;
	case PROP_AMOUNT:
		g_value_set_double(val, self->amount);
		break;
	case PROP_TAX:
		g_value_set_double(val, self->tax);
		break;
	case PROP_TAX_AMOUNT:
		g_value_set_double(val, self->tax_amount);
		break;
	case PROP_TAX2:
		g_value_set_double(val, self->tax2);
		break;
	case PROP_TAX2_AMOUNT:
		g_value_set_double(val, self->tax2_amount);
		break;
	case PROP_DISCOUNT:
		g_value_set_double(val, self->discount);
		break;
	case PROP_DISCOUNT_AMOUNT:
		g_value_set_double(val, self->discount_amount);
		break;
	case PROP_SUBJECT:
		g_value_set_string(val, self->subject);
		break;
	case PROP_NOTES:
		g_value_set_string(val, self->notes);
		break;
	case PROP_CURRENCY:
		g_value_set_string(val, self->currency);
		break;
	case PROP_STATE:
		g_value_set_int(val, self->state);
		break;
	case PROP_ISSUE_DATE:
		g_value_set_boxed(val, self->issue_date);
		break;
	case PROP_SENT_AT:
		g_value_set_boxed(val, self->sent_at);
		break;
	case PROP_ACCEPTED_AT:
		g_value_set_boxed(val, self->accepted_at);
		break;
	case PROP_DECLINED_AT:
		g_value_set_boxed(val, self->declined_at);
		break;
	case PROP_CREATED_AT:
		g_value_set_boxed(val, self->created_at);
		break;
	case PROP_UPDATED_AT:
		g_value_set_boxed(val, self->updated_at);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_estimate_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestEstimate *self = HARVEST_ESTIMATE(obj);

	switch (prop_id) {
	case PROP_ID:
		self->id = g_value_get_int(val);
		break;
	case PROP_CLIENT:
		if (self->client != NULL)
			g_object_unref(self->client);
		self->client = g_value_dup_object(val);
		break;
	case PROP_LINE_ITEMS:
		if (self->line_items != NULL) {
			/**
			 * Always remove the last one so that the array does not need to be resized. If we
			 * always remove the last one in conjunction with g_ptr_array_remove_index_fast, then
			 * the reverse order of the array should be preserved and we will be able to iterate
			 * linearly.
			 */
			for (int i = self->line_items->len - 1; i >= 0; i--) {
				HarvestEstimateLineItem *line_item =
					HARVEST_ESTIMATE_LINE_ITEM(g_ptr_array_remove_index_fast(self->line_items, i));
				if (line_item != NULL)
					g_object_unref(line_item);
			}
			g_ptr_array_unref(self->line_items);
		}
		self->line_items = g_value_dup_boxed(val);
		break;
	case PROP_CREATOR:
		if (self->creator != NULL)
			g_object_unref(self->client);
		self->creator = g_value_dup_object(val);
		break;
	case PROP_CLIENT_KEY:
		g_free(self->client_key);
		self->client_key = g_value_dup_string(val);
		break;
	case PROP_NUMBER:
		g_free(self->number);
		self->number = g_value_dup_string(val);
		break;
	case PROP_PURCHASE_ORDER:
		g_free(self->purchase_order);
		self->purchase_order = g_value_dup_string(val);
		break;
	case PROP_AMOUNT:
		self->amount = g_value_get_double(val);
		break;
	case PROP_TAX:
		self->tax = g_value_get_double(val);
		break;
	case PROP_TAX_AMOUNT:
		self->tax_amount = g_value_get_double(val);
		break;
	case PROP_TAX2:
		self->tax2 = g_value_get_double(val);
		break;
	case PROP_TAX2_AMOUNT:
		self->tax2_amount = g_value_get_double(val);
		break;
	case PROP_DISCOUNT:
		self->discount = g_value_get_double(val);
		break;
	case PROP_DISCOUNT_AMOUNT:
		self->discount_amount = g_value_get_double(val);
		break;
	case PROP_SUBJECT:
		g_free(self->subject);
		self->subject = g_value_dup_string(val);
		break;
	case PROP_NOTES:
		g_free(self->notes);
		self->notes = g_value_dup_string(val);
		break;
	case PROP_CURRENCY:
		g_free(self->currency);
		self->currency = g_value_dup_string(val);
		break;
	case PROP_STATE:
		self->state = g_value_get_int(val);
		break;
	case PROP_ISSUE_DATE:
		if (self->issue_date != NULL)
			g_date_time_unref(self->issue_date);
		self->issue_date = g_value_dup_boxed(val);
		break;
	case PROP_SENT_AT:
		if (self->sent_at != NULL)
			g_date_time_unref(self->sent_at);
		self->sent_at = g_value_dup_boxed(val);
		break;
	case PROP_ACCEPTED_AT:
		if (self->accepted_at != NULL)
			g_date_time_unref(self->accepted_at);
		self->accepted_at = g_value_dup_boxed(val);
		break;
	case PROP_DECLINED_AT:
		if (self->declined_at != NULL)
			g_date_time_unref(self->declined_at);
		self->declined_at = g_value_dup_boxed(val);
		break;
	case PROP_CREATED_AT:
		if (self->created_at != NULL)
			g_date_time_unref(self->created_at);
		self->created_at = g_value_dup_boxed(val);
		break;
	case PROP_UPDATED_AT:
		if (self->updated_at != NULL)
			g_date_time_unref(self->updated_at);
		self->updated_at = g_value_dup_boxed(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_estimate_class_init(HarvestEstimateClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_estimate_finalize;
	obj_class->get_property = harvest_estimate_get_property;
	obj_class->set_property = harvest_estimate_set_property;

	obj_properties[PROP_ID] = g_param_spec_int("id", _("ID"), _("Unique ID for the estimate."), 0,
		INT_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_CLIENT] = g_param_spec_object("client", _("Client"),
		_("An object containing estimate’s client id and name."), HARVEST_TYPE_CLIENT,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_LINE_ITEMS] =
		g_param_spec_boxed("line_items", _("Line Items"), _("Array of estimate line items."),
			G_TYPE_PTR_ARRAY, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_CREATOR]		= g_param_spec_object("creator", _("Creator"),
		   _("An object containing the id and name of the person that created the estimate."),
		   HARVEST_TYPE_CREATOR, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_CLIENT_KEY]		= g_param_spec_string("client_key", _("Client Key"),
		_("Used to build a URL to the public web invoice for your client: "
		  "https://{ACCOUNT_SUBDOMAIN}.harvestapp.com/client/estimates/abc123456"),
		NULL, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_NUMBER]			= g_param_spec_string("number", _("Number"),
		_("If no value is set, the number will be automatically generated."), NULL,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_PURCHASE_ORDER] = g_param_spec_string("purchase_order", _("Purshase Order"),
		_("The purchase order number."), NULL, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_AMOUNT]			= g_param_spec_double("amount", _("Amount"),
		_("The total amount for the estimate, including any discounts and taxes."), 0, DBL_MAX, 0,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_TAX]			= g_param_spec_double("tax", _("Tax"),
		   _("This percentage is applied to the subtotal, including line items and discounts."), 0,
		   DBL_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_TAX_AMOUNT]		= g_param_spec_double("tax_amount", _("Tax Amount"),
		_("The first amount of tax included, calculated from tax. If no tax is defined, this value "
		  "will be null."),
		0, DBL_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_TAX2]			= g_param_spec_double("tax2", _("Tax 2"),
		  _("This percentage is applied to the subtotal, including line items and discounts."), 0,
		  DBL_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_TAX2_AMOUNT] =
		g_param_spec_double("tax2_amount", _("Tax2 Amount"), _("The amount calculated from tax2."),
			0, DBL_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_DISCOUNT]		 = g_param_spec_double("discount", _("Discount"),
		   _("This percentage is subtracted from the subtotal."), 0, DBL_MAX, 0,
		   G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_DISCOUNT_AMOUNT] = g_param_spec_double("discount_amount",
		_("Discount Amount"), _("The amount calcuated from discount."), 0, DBL_MAX, 0,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_SUBJECT]		 = g_param_spec_string("subject", _("Subject"),
		_("The estimate subject."), NULL, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_NOTES]			 = g_param_spec_string("notes", _("Notes"),
		  _("Any additional notes included on the estimate."), NULL,
		  G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_CURRENCY]		 = g_param_spec_string("currency", _("Currency"),
		   _("The currency code associated with this estimate."), NULL,
		   G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_STATE]			 = g_param_spec_int("state", _("State"),
		  _("The current state of the estimate: draft, sent, accepted, or declined."),
		  HARVEST_ESTIMATE_STATE_DRAFT, HARVEST_ESTIMATE_STATE_DECLINED, HARVEST_ESTIMATE_STATE_DRAFT,
		  G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_ISSUE_DATE] =
		g_param_spec_boxed("issue_date", _("Issue Date"), _("Date the estimate was issued."),
			G_TYPE_DATE_TIME, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_SENT_AT] =
		g_param_spec_boxed("sent_at", _("Sent At"), _("Date and time the estimate was sent."),
			G_TYPE_DATE_TIME, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_ACCEPTED_AT] = g_param_spec_boxed("accepted_at", _("Accepted At"),
		_("Date and time the estimate was accepted."), G_TYPE_DATE_TIME,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_DECLINED_AT] = g_param_spec_boxed("declined_at", _("Declined At"),
		_("Date and time the estimate was declined."), G_TYPE_DATE_TIME,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_CREATED_AT]  = g_param_spec_boxed("created_at", _("Created At"),
		 _("Date and time the estimate was created."), G_TYPE_DATE_TIME,
		 G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_UPDATED_AT]  = g_param_spec_boxed("updated_at", _("Updated At"),
		 _("Date and time the estimate was last updated."), G_TYPE_DATE_TIME,
		 G_PARAM_CONSTRUCT | G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_estimate_init(G_GNUC_UNUSED HarvestEstimate *self)
{}
