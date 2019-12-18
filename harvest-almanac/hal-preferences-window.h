#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>
#include <handy.h>
#include <libsecret/secret.h>
#include <libsoup/soup.h>

G_BEGIN_DECLS

#define HAL_SECRET_SCHEMA (hal_get_secret_schema())

#define HAL_DEFAULT_SOUP_MAX_CONNECTIONS 4
#define HAL_DEFAULT_SOUP_LOGGER_LEVEL SOUP_LOGGER_LOG_NONE

#define HAL_TYPE_PREFERENCES_WINDOW (hal_preferences_window_get_type())
G_DECLARE_FINAL_TYPE(
	HalPreferencesWindow, hal_preferences_window, HAL, PREFERENCES_WINDOW, HdyPreferencesWindow)

HalPreferencesWindow *hal_preferences_window_new(GSettings *settings) G_GNUC_WARN_UNUSED_RESULT;
const SecretSchema *hal_get_secret_schema(void) G_GNUC_CONST;

G_END_DECLS
