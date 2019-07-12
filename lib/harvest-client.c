#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-client.h"

struct _HarvestClient
{
	GObject parent_instance;

	int id;
	char *name;
	gboolean is_active : 1;
	char *address;
	char *currency;
	GDateTime *created_at;
	GDateTime *updated_at;
};

G_DEFINE_TYPE(HarvestClient, harvest_client, G_TYPE_OBJECT)

enum HarvestClientProps
{
	PROP_0,
	PROP_ID,
	PROP_NAME,
	PROP_IS_ACTIVE,
	PROP_ADDRESS,
	PROP_CURRENCY,
	PROP_CREATED_AT,
	PROP_UPDATED_AT,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_client_finalize(GObject *obj)
{
	HarvestClient *self = HARVEST_CLIENT(obj);

	g_free(self->name);
	g_free(self->address);
	g_free(self->currency);
	if (self->created_at != NULL)
		g_date_time_unref(self->created_at);
	if (self->updated_at != NULL)
		g_date_time_unref(self->updated_at);

	G_OBJECT_CLASS(harvest_client_parent_class)->finalize(obj);
}

static void
harvest_client_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestClient *self = HARVEST_CLIENT(obj);

	switch (prop_id) {
	case PROP_ID:
		g_value_set_int(val, self->id);
		break;
	case PROP_NAME:
		g_value_set_string(val, self->name);
		break;
	case PROP_IS_ACTIVE:
		g_value_set_boolean(val, self->is_active);
		break;
	case PROP_ADDRESS:
		g_value_set_string(val, self->address);
		break;
	case PROP_CURRENCY:
		g_value_set_string(val, self->currency);
		break;
	case PROP_CREATED_AT:
		g_value_set_boxed(val, self->created_at);
		break;
	case PROP_UPDATED_AT:
		g_value_set_boxed(val, self->updated_at);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
harvest_client_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestClient *self = HARVEST_CLIENT(obj);

	switch (prop_id) {
	case PROP_ID:
		self->id = g_value_get_int(val);
		break;
	case PROP_NAME:
		g_free(self->name);
		self->name = g_value_dup_string(val);
		break;
	case PROP_IS_ACTIVE:
		self->is_active = g_value_get_boolean(val);
		break;
	case PROP_ADDRESS:
		g_free(self->address);
		self->address = g_value_dup_string(val);
		break;
	case PROP_CURRENCY:
		g_free(self->currency);
		self->currency = g_value_dup_string(val);
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
		break;
	}
}

static void
harvest_client_class_init(HarvestClientClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_client_finalize;
	obj_class->get_property = harvest_client_get_property;
	obj_class->set_property = harvest_client_set_property;

	obj_properties[PROP_ID]   = g_param_spec_int("id", _("ID"), _("Unique ID for the client."), 0,
		  INT_MAX, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_NAME] = g_param_spec_string("name", _("Name"),
		_("A textual description of the client."), NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_IS_ACTIVE]  = g_param_spec_boolean("is_active", _("Is Active"),
		 _("Whether the client is active or archived."), TRUE,
		 G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_ADDRESS]	= g_param_spec_string("address", _("Address"),
		   _("The physical address for the client."), NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_CURRENCY]   = g_param_spec_string("currency", _("Currency"),
		  _("The currency code associated with this client."), NULL,
		  G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_CREATED_AT] = g_param_spec_boxed("created_at", _("Created At"),
		_("Date and time the client was created."), G_TYPE_DATE_TIME,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_UPDATED_AT] = g_param_spec_boxed("updated_at", _("Updated At"),
		_("Date and time the client was last updated."), G_TYPE_DATE_TIME,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_client_init(G_GNUC_UNUSED HarvestClient *self)
{}
