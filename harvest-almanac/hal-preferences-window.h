#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>
#include <handy.h>
#include <libsecret/secret.h>

#define HAL_SECRET_SCHEMA (hal_get_secret_schema())
#define HAL_TYPE_PREFERENCES_WINDOW (hal_preferences_window_get_type())
G_DECLARE_FINAL_TYPE(
	HalPreferencesWindow, hal_preferences_window, HAL, PREFERENCES_WINDOW, HdyPreferencesWindow)

G_BEGIN_DECLS

const SecretSchema *hal_get_secret_schema(void) G_GNUC_CONST;

HalPreferencesWindow *hal_preferences_window_new(GSettings *settings) G_GNUC_WARN_UNUSED_RESULT;

G_END_DECLS
