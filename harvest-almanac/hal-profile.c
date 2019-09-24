#include "config.h"

#define G_LOG_DOMAIN "HalProfile"

#include <glib-object.h>
#include <gtk/gtk.h>

#include "hal-profile.h"

struct _HalProfile
{
	GtkStack parent_instance;
};

typedef struct HalProfilePrivate
{
	GtkLabel *name;
	GtkLabel *email;
	GtkLabel *phone;
	GtkLabel *roles;
	GtkLabel *timezone;
} HalProfilePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalProfile, hal_profile, GTK_TYPE_STACK)

enum HalProfileProps
{
	PROP_0,
	PROP_USER,
	N_PROPS
};

G_GNUC_UNUSED static GParamSpec *obj_properties[N_PROPS];

static void
hal_profile_finalize(GObject *self)
{
	G_OBJECT_CLASS(hal_profile_parent_class)->finalize(self);
}

static void
hal_profile_get_property(GObject *obj, guint prop_id, G_GNUC_UNUSED GValue *val, GParamSpec *pspec)
{
	switch (prop_id) {
	case PROP_USER:
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
hal_profile_set_property(
	GObject *obj, guint prop_id, G_GNUC_UNUSED const GValue *val, GParamSpec *pspec)
{
	switch (prop_id) {
	case PROP_USER:
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
hal_profile_class_init(HalProfileClass *klass)
{
	GObjectClass *obj_class   = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);

	obj_class->finalize		= hal_profile_finalize;
	obj_class->get_property = hal_profile_get_property;
	obj_class->set_property = hal_profile_set_property;

	// TODO: actually create GParamSpecs

	gtk_widget_class_set_template_from_resource(
		wid_class, "/io/partin/tristan/HarvestAlmanac/ui/hal-profile.ui");
	gtk_widget_class_bind_template_child_private(wid_class, HalProfile, name);
	gtk_widget_class_bind_template_child_private(wid_class, HalProfile, email);
	gtk_widget_class_bind_template_child_private(wid_class, HalProfile, phone);
	gtk_widget_class_bind_template_child_private(wid_class, HalProfile, roles);
	gtk_widget_class_bind_template_child_private(wid_class, HalProfile, timezone);
}

static void
hal_profile_init(HalProfile *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}

HalProfile *
hal_profile_new()
{
	return g_object_new(HAL_TYPE_PROFILE, NULL);
}
