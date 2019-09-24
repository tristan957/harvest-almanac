#include "config.h"

#define G_LOG_DOMAIN "HalPreferencesWindow"

#include <glib-object.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <handy.h>
#include <libsecret/secret.h>

#include "hal-preferences-window.h"

struct _HalPreferencesWindow
{
	HdyPreferencesWindow parent_instance;

	GSettings *settings;
};

typedef struct HalPreferencesWindowPrivate
{
	GtkEntry *harvest_api_access_token_entry;
	GtkEntry *harvest_api_contact_email;
	GtkSwitch *prefer_dark_theme_switch;
	GtkSpinButton *soup_max_connections_spin;
	GtkComboBoxText *soup_logger_level_combo;
} HalPreferencesWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(
	HalPreferencesWindow, hal_preferences_window, HDY_TYPE_PREFERENCES_WINDOW)

enum HalPreferencesWindowProps
{
	PROP_0,
	PROP_SETTINGS,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

const SecretSchema *
hal_get_secret_schema(void)
{
	static const SecretSchema *hal_schema = {"io.partin.tristan.HarvestAlmanac", SECRET_SCHEMA_NONE,
		{{"email", SECRET_SCHEMA_ATTRIBUTE_STRING}}};

	return &hal_schema;
}

static void
on_prefer_dark_theme_switch_activate(
	GtkSwitch *widget, G_GNUC_UNUSED GParamSpec *pspec, gpointer user_data)
{
	HalPreferencesWindow *self = HAL_PREFERENCES_WINDOW(user_data);

	g_settings_set_boolean(self->settings, "prefer-dark-theme", gtk_switch_get_active(widget));
}

static void
hal_preferences_window_constructed(GObject *obj)
{
	HalPreferencesWindow *self		  = HAL_PREFERENCES_WINDOW(obj);
	HalPreferencesWindowPrivate *priv = hal_preferences_window_get_instance_private(self);

	g_autofree const char *key = g_settings_get_string(self->settings, "harvest-api-access-token");

	gtk_entry_set_text(priv->harvest_api_access_token_entry, key);
	gtk_switch_set_active(priv->prefer_dark_theme_switch,
		g_settings_get_boolean(self->settings, "prefer-dark-theme"));
}

static void
hal_preferences_window_finalize(GObject *obj)
{
	HalPreferencesWindow *self = HAL_PREFERENCES_WINDOW(obj);

	g_object_unref(self->settings);

	G_OBJECT_CLASS(hal_preferences_window_parent_class)->finalize(obj);
}

static void
hal_preferences_window_set_property(
	GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HalPreferencesWindow *self = HAL_PREFERENCES_WINDOW(obj);

	switch (prop_id) {
	case PROP_SETTINGS:
		if (self->settings != NULL) {
			g_object_unref(self->settings);
		}
		self->settings = g_value_dup_object(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
hal_preferences_window_class_init(HalPreferencesWindowClass *klass)
{
	GObjectClass *obj_class   = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);

	obj_class->constructed  = hal_preferences_window_constructed;
	obj_class->finalize		= hal_preferences_window_finalize;
	obj_class->set_property = hal_preferences_window_set_property;

	obj_properties[PROP_SETTINGS]
		= g_param_spec_object("settings", _("Settings"), _("Application settings"), G_TYPE_SETTINGS,
			G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);

	gtk_widget_class_set_template_from_resource(
		wid_class, "/io/partin/tristan/HarvestAlmanac/ui/hal-preferences-window.ui");
	gtk_widget_class_bind_template_child_private(
		wid_class, HalPreferencesWindow, harvest_api_access_token_entry);
	gtk_widget_class_bind_template_child_private(
		wid_class, HalPreferencesWindow, harvest_api_contact_email);
	gtk_widget_class_bind_template_child_private(
		wid_class, HalPreferencesWindow, prefer_dark_theme_switch);
	gtk_widget_class_bind_template_child_private(
		wid_class, HalPreferencesWindow, soup_max_connections_spin);
	gtk_widget_class_bind_template_child_private(
		wid_class, HalPreferencesWindow, soup_logger_level_combo);
	gtk_widget_class_bind_template_callback(wid_class, on_prefer_dark_theme_switch_activate);
}

static void
hal_preferences_window_init(HalPreferencesWindow *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}

HalPreferencesWindow *
hal_preferences_window_new(GtkWindow *parent, GSettings *settings)
{
	return g_object_new(
		HAL_TYPE_PREFERENCES_WINDOW, "transient-for", parent, "settings", settings, NULL);
}
