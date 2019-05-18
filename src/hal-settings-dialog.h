#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>
#include <handy.h>

G_BEGIN_DECLS

#define HAL_TYPE_SETTINGS_DIALOG (hal_settings_dialog_get_type())
G_DECLARE_FINAL_TYPE(HalSettingsDialog, hal_settings_dialog, HAL, SETTINGS_DIALOG, HdyDialog)

HalSettingsDialog *hal_settings_dialog_new(GtkWindow *parent);

G_END_DECLS
