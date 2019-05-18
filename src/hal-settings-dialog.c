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
	GtkEntry *harvest_api_key_entry;
} HalSettingsDialogPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalSettingsDialog, hal_settings_dialog, HDY_TYPE_DIALOG)

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
}

static void
hal_settings_dialog_init(HalSettingsDialog *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}

HalSettingsDialog *
hal_settings_dialog_new(GtkWindow *parent)
{
	return g_object_new(HAL_TYPE_SETTINGS_DIALOG, "transient-for", parent, NULL);
}
