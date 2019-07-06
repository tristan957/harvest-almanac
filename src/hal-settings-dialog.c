#include <glib-object.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <handy.h>

#include "hal-config.h"
#include "hal-settings-dialog.h"

struct _HalSettingsDialog
{
	HdyDialog parent_instance;
};

typedef struct HalSettingsDialogPrivate
{
	GtkButton *save_button;
	GtkEntry *harvest_api_key_entry;
	GtkSwitch *prefer_dark_theme_switch;

	GSettings *settings;
	gboolean dirty;
	gboolean initial_state;
} HalSettingsDialogPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalSettingsDialog, hal_settings_dialog, HDY_TYPE_DIALOG)

enum HalSettingsDialogProps
{
	PROP_0,
	PROP_SETTINGS,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
hal_settings_dialog_set_dirty(HalSettingsDialog *self)
{
	HalSettingsDialogPrivate *priv = hal_settings_dialog_get_instance_private(self);

	if (priv->dirty) {
		return;
	}

	if (!priv->initial_state) {
		priv->dirty = TRUE;
		gtk_widget_set_sensitive(GTK_WIDGET(priv->save_button), TRUE);
	}
}

static void
on_prefer_dark_theme_switch_activate(
	GtkSwitch *widget, G_GNUC_UNUSED GParamSpec *pspec, gpointer user_data)
{
	HalSettingsDialog *self		   = HAL_SETTINGS_DIALOG(user_data);
	HalSettingsDialogPrivate *priv = hal_settings_dialog_get_instance_private(self);

	hal_settings_dialog_set_dirty(self);

	g_settings_set_boolean(
		priv->settings, SETTINGS_PREFER_DARK_THEME, gtk_switch_get_active(widget));
}

static void
on_harvest_api_key_entry_changed(G_GNUC_UNUSED GtkEntry *widget, gpointer user_data)
{
	hal_settings_dialog_set_dirty(HAL_SETTINGS_DIALOG(user_data));
}

static void
on_save_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	HalSettingsDialog *self		   = HAL_SETTINGS_DIALOG(user_data);
	HalSettingsDialogPrivate *priv = hal_settings_dialog_get_instance_private(self);

	if (!priv->dirty) {
		return;
	}

	g_autofree gchar *text = g_strdup(gtk_entry_get_text(priv->harvest_api_key_entry));
	g_settings_set_string(priv->settings, SETTINGS_HARVEST_API_KEY, g_strstrip(text));
	g_settings_apply(priv->settings);
	g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme",
		g_settings_get_boolean(priv->settings, SETTINGS_PREFER_DARK_THEME), NULL);
}

static void
hal_settings_dialog_constructed(GObject *obj)
{
	HalSettingsDialog *self		   = HAL_SETTINGS_DIALOG(obj);
	HalSettingsDialogPrivate *priv = hal_settings_dialog_get_instance_private(self);

	g_autofree const gchar *key = g_settings_get_string(priv->settings, SETTINGS_HARVEST_API_KEY);

	gtk_entry_set_text(priv->harvest_api_key_entry, key);
	gtk_switch_set_active(priv->prefer_dark_theme_switch,
		g_settings_get_boolean(priv->settings, SETTINGS_PREFER_DARK_THEME));

	priv->initial_state = FALSE;
}

static void
hal_settings_dialog_finalize(GObject *obj)
{
	HalSettingsDialog *self		   = HAL_SETTINGS_DIALOG(obj);
	HalSettingsDialogPrivate *priv = hal_settings_dialog_get_instance_private(self);

	g_object_unref(priv->settings);

	G_OBJECT_CLASS(hal_settings_dialog_parent_class)->finalize(obj);
}

static void
hal_settings_dialog_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HalSettingsDialog *self		   = HAL_SETTINGS_DIALOG(obj);
	HalSettingsDialogPrivate *priv = hal_settings_dialog_get_instance_private(self);

	switch (prop_id) {
	case PROP_SETTINGS:
		if (priv->settings != NULL) {
			g_object_unref(priv->settings);
		}
		priv->settings = g_value_dup_object(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
hal_settings_dialog_class_init(HalSettingsDialogClass *klass)
{
	GObjectClass *obj_class   = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);

	obj_class->constructed  = hal_settings_dialog_constructed;
	obj_class->finalize		= hal_settings_dialog_finalize;
	obj_class->set_property = hal_settings_dialog_set_property;

	obj_properties[PROP_SETTINGS] =
		g_param_spec_object("settings", _("Settings"), _("Application settings"), G_TYPE_SETTINGS,
			G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_PRIVATE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);

	gtk_widget_class_set_template_from_resource(
		wid_class, "/io/partin/tristan/HarvestAlmanac/ui/hal-settings-dialog.ui");
	gtk_widget_class_bind_template_child_private(
		wid_class, HalSettingsDialog, harvest_api_key_entry);
	gtk_widget_class_bind_template_child_private(
		wid_class, HalSettingsDialog, prefer_dark_theme_switch);
	gtk_widget_class_bind_template_child_private(wid_class, HalSettingsDialog, save_button);
	gtk_widget_class_bind_template_callback(wid_class, on_harvest_api_key_entry_changed);
	gtk_widget_class_bind_template_callback(wid_class, on_prefer_dark_theme_switch_activate);
	gtk_widget_class_bind_template_callback(wid_class, on_save_button_clicked);
}

static void
hal_settings_dialog_init(HalSettingsDialog *self)
{
	HalSettingsDialogPrivate *priv = hal_settings_dialog_get_instance_private(self);

	gtk_widget_init_template(GTK_WIDGET(self));

	priv->dirty			= FALSE;
	priv->initial_state = TRUE;
}

HalSettingsDialog *
hal_settings_dialog_new(GtkWindow *parent, GSettings *settings)
{
	return g_object_new(
		HAL_TYPE_SETTINGS_DIALOG, "transient-for", parent, "settings", settings, NULL);
}
