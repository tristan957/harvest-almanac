#include <glib-object.h>
#include <gtk/gtk.h>
#include <handy.h>

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

	gboolean dirty;
} HalSettingsDialogPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalSettingsDialog, hal_settings_dialog, HDY_TYPE_DIALOG)

static void
on_prefer_dark_theme_switch_activate(G_GNUC_UNUSED GtkSwitch *widget,
									 G_GNUC_UNUSED GParamSpec *pspec, gpointer user_data)
{
	HalSettingsDialog *self		   = HAL_SETTINGS_DIALOG(user_data);
	HalSettingsDialogPrivate *priv = hal_settings_dialog_get_instance_private(self);

	priv->dirty = TRUE;
	gtk_widget_set_sensitive(GTK_WIDGET(priv->save_button), TRUE);
}

static void
on_save_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	HalSettingsDialog *self		   = HAL_SETTINGS_DIALOG(user_data);
	HalSettingsDialogPrivate *priv = hal_settings_dialog_get_instance_private(self);

	if (!priv->dirty) {
		return;
	}
}

static void
hal_settings_dialog_finalize(GObject *obj)
{
	G_OBJECT_CLASS(hal_settings_dialog_parent_class)->finalize(obj);
}

static void
hal_settings_dialog_class_init(HalSettingsDialogClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class  = GTK_WIDGET_CLASS(klass);

	object_class->finalize = hal_settings_dialog_finalize;

	gtk_widget_class_set_template_from_resource(
		wid_class, "/io/partin/tristan/HarvestAlmanac/ui/hal-settings-dialog.ui");
	gtk_widget_class_bind_template_child_private(wid_class, HalSettingsDialog,
												 harvest_api_key_entry);
	gtk_widget_class_bind_template_child_private(wid_class, HalSettingsDialog,
												 prefer_dark_theme_switch);
	gtk_widget_class_bind_template_child_private(wid_class, HalSettingsDialog, save_button);
	gtk_widget_class_bind_template_callback(wid_class, on_prefer_dark_theme_switch_activate);
	gtk_widget_class_bind_template_callback(wid_class, on_save_button_clicked);
}

static void
hal_settings_dialog_init(HalSettingsDialog *self)
{
	HalSettingsDialogPrivate *priv = hal_settings_dialog_get_instance_private(self);

	gtk_widget_init_template(GTK_WIDGET(self));

	priv->dirty = FALSE;
}

HalSettingsDialog *
hal_settings_dialog_new(GtkWindow *parent)
{
	return g_object_new(HAL_TYPE_SETTINGS_DIALOG, "transient-for", parent, NULL);
}
