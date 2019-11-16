#include "config.h"

#define G_LOG_DOMAIN "HalContext"

#include <gio/gio.h>
#include <glib-object.h>
#include <glib/gi18n.h>
#include <harvest-glib/harvest.h>

#include "hal-context.h"

HalContext *instance;

struct _HalContext
{
	GObject parent_instance;

	// The currently logged in user
	HarvestUser *user;
};

G_DEFINE_TYPE(HalContext, hal_context, G_TYPE_OBJECT)

enum HalContextProps
{
	PROP_0,
	PROP_USER,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
hal_context_finalize(GObject *obj)
{
	HalContext *self = HAL_CONTEXT(obj);

	g_clear_object(&self->user);

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
		self->user = g_value_dup_object(val);
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

	obj_properties[PROP_USER]
		= g_param_spec_object("user", _("User"), _("Currently logged in user of the application."),
			HARVEST_TYPE_USER, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
hal_context_init(G_GNUC_UNUSED HalContext *self)
{}

void
hal_context_initialize()
{
	instance = g_object_new(HAL_TYPE_CONTEXT, NULL);
}

const HalContext *
hal_context_get()
{
	return instance;
}

HarvestUser *
hal_context_get_user()
{
	g_return_val_if_fail(instance != NULL, NULL);

	return instance->user;
}

void
hal_context_set_user(HarvestUser *user)
{
	g_return_if_fail(HARVEST_IS_USER(user) && instance != NULL);

	GValue val = G_VALUE_INIT;
	g_value_init(&val, HARVEST_TYPE_USER);
	g_value_set_object(&val, user);

	g_object_set_property(G_OBJECT(instance), "user", &val);
}
