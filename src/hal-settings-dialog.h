#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>
#include <handy.h>

#define HAL_TYPE_SETTINGS_DIALOG (hal_settings_dialog_get_type())
G_DECLARE_FINAL_TYPE(HalSettingsDialog, hal_settings_dialog, HAL, SETTINGS_DIALOG, HdyDialog)

G_BEGIN_DECLS

HalSettingsDialog *hal_settings_dialog_new(GtkWindow *parent, GSettings *settings);

G_END_DECLS
