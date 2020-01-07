#include "config.h"

#define G_LOG_DOMAIN "HalContext"

#include <gio/gio.h>
#include <glib-object.h>
#include <glib/gi18n.h>
#include <harvest-glib/harvest.h>

#include "hal-context.h"

HalContext *CONTEXT;

struct _HalContext
{
	GObject parent_instance;

	// The currently logged in user
	HarvestUser *user;
	// The company of the currently logged in user
	HarvestCompany *company;
};

G_DEFINE_TYPE(HalContext, hal_context, G_TYPE_OBJECT)

enum HalContextProps
{
	PROP_0,
	PROP_USER,
	PROP_COMPANY,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
hal_context_finalize(GObject *obj)
{
	HalContext *self = HAL_CONTEXT(obj);

	/**
	 * Using g_clear_object here due to the ownership model. CONTEXT will be the last thing to be
	 * garbage collected before the application itself.
	 */
	g_clear_object(&self->user);
	g_clear_object(&self->company);

	G_OBJECT_CLASS(hal_context_parent_class)->finalize(obj);
}

static void
hal_context_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HalContext *self = HAL_CONTEXT(obj);

	switch (prop_id) {
	case PROP_USER:
		g_value_set_object(val, self->user);
		break;
	case PROP_COMPANY:
		g_value_set_object(val, self->company);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
hal_context_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HalContext *self = HAL_CONTEXT(obj);

	switch (prop_id) {
	case PROP_USER:
		if (self->user != NULL)
			g_object_unref(self->user);
		self->user = g_value_dup_object(val);
		break;
	case PROP_COMPANY:
		if (self->company != NULL)
			g_object_unref(self->company);
		self->company = g_value_dup_object(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
hal_context_class_init(HalContextClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= hal_context_finalize;
	obj_class->get_property = hal_context_get_property;
	obj_class->set_property = hal_context_set_property;

	obj_properties[PROP_USER]	 = g_param_spec_object("user", _("User"),
		   _("Currently logged in user of the application."), HARVEST_TYPE_USER,
		   G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_COMPANY] = g_param_spec_object("company", _("Company"),
		_("Company of the currently logged in user."), HARVEST_TYPE_COMPANY,
		G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
hal_context_init(G_GNUC_UNUSED HalContext *self)
{}

void
hal_context_initialize()
{
	CONTEXT = g_object_new(HAL_TYPE_CONTEXT, NULL);
}

void
hal_context_set_user(HarvestUser *user)
{
	g_return_if_fail(CONTEXT != NULL && HARVEST_IS_USER(user));

	CONTEXT->user = user;

	g_object_notify_by_pspec(G_OBJECT(CONTEXT), obj_properties[PROP_USER]);
}

HarvestUser *
hal_context_get_user()
{
	return CONTEXT->user;
}

void
hal_context_set_company(HarvestCompany *company)
{
	g_return_if_fail(CONTEXT != NULL && HARVEST_IS_COMPANY(company));

	CONTEXT->company = company;

	g_object_notify_by_pspec(G_OBJECT(CONTEXT), obj_properties[PROP_COMPANY]);
}

HarvestCompany *
hal_context_get_company()
{
	return CONTEXT->company;
}
