#include "config.h"

#define G_LOG_DOMAIN "HalApplication"

#include <string.h>

#include <glib-object.h>
#include <gtk/gtk.h>
#include <harvest.h>
#include <libsoup/soup.h>

#include "hal-application.h"
#include "hal-preferences-window.h"
#include "hal-resources.h"
#include "hal-time-entry.h"
#include "hal-window.h"

#define HAL_MAX_CONNS_PER_HOST 4

struct _HalApplication
{
	GtkApplication parent_instance;

	GSettings *settings;
	HarvestApiClient *client;
};

typedef struct HalApplicationPrivate
{
	HalWindow *main_window;
} HalApplicationPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalApplication, hal_application, GTK_TYPE_APPLICATION)

static void
hal_application_activate(GApplication *app)
{
	HalApplication *self		= HAL_APPLICATION(app);
	HalApplicationPrivate *priv = hal_application_get_instance_private(HAL_APPLICATION(self));

	if (priv->main_window == NULL) {
		priv->main_window = hal_window_new(app);
	}

	g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme",
		g_settings_get_boolean(self->settings, "prefer-dark-theme"), NULL);

	gtk_window_present(GTK_WINDOW(priv->main_window));
}

static void
hal_application_about(
	G_GNUC_UNUSED GSimpleAction *action, G_GNUC_UNUSED GVariant *param, gpointer data)
{
	static const char *authors[] = {"Tristan Partin"};

	HalApplication *self		= HAL_APPLICATION(data);
	HalApplicationPrivate *priv = hal_application_get_instance_private(self);

	gtk_show_about_dialog(GTK_WINDOW(priv->main_window), "program-name", PACKAGE_NAME, "version",
		PACKAGE_VERSION, "license-type", PACKAGE_LICENSE, "website", PACKAGE_WEBSITE,
		"website-label", PACKAGE_WEBSITE_LABEL, "authors", authors, "logo-icon-name", "trophy-gold",
		NULL);
}

static void
hal_application_preferences(
	G_GNUC_UNUSED GSimpleAction *action, G_GNUC_UNUSED GVariant *param, gpointer data)
{
	HalApplication *self		= HAL_APPLICATION(data);
	HalApplicationPrivate *priv = hal_application_get_instance_private(self);

	HalPreferencesWindow *preferences_window
		= hal_preferences_window_new(GTK_WINDOW(priv->main_window), self->settings);
	gtk_window_present(GTK_WINDOW(preferences_window));
}

static void
hal_application_time_entry_start(
	G_GNUC_UNUSED GSimpleAction *action, GVariant *param, gpointer data)
{
	const guint64 address			  = g_variant_get_uint64(param);
	G_GNUC_UNUSED HalTimeEntry *entry = HAL_TIME_ENTRY((HalTimeEntry *) address);

	g_autoptr(GNotification) notification = g_notification_new("Harvest Almanac");
	g_notification_set_body(notification, "Client -- Project timer started");
	g_notification_add_button_with_target(
		notification, "Stop Timer", "app.time-entry-stop", "t", address, NULL);
	g_application_send_notification(G_APPLICATION(data), "time-entry", notification);
}

static void
hal_application_time_entry_stop(G_GNUC_UNUSED GSimpleAction *action, GVariant *param, gpointer data)
{
	const guint64 address = g_variant_get_uint64(param);
	HalTimeEntry *entry   = HAL_TIME_ENTRY((HalTimeEntry *) address);

	hal_time_entry_stop(entry);
	g_application_withdraw_notification(G_APPLICATION(data), "time-entry");
}

static void
hal_application_create_client(
	G_GNUC_UNUSED GSimpleAction *action, G_GNUC_UNUSED GVariant *param, gpointer data)
{
	HalApplication *self = HAL_APPLICATION(data);

	g_autofree const char *harvest_api_access_token
		= g_settings_get_string(self->settings, "harvest-api-access-token");
	g_autofree const char *harvest_api_contact_email
		= g_settings_get_string(self->settings, "harvest-api-contact-email");
	const unsigned int harvest_account_id
		= g_settings_get_uint(self->settings, "harvest-account-id");
	const unsigned int logger_level = g_settings_get_uint(self->settings, "soup-logger-level");

	SoupLogger *logger = soup_logger_new(logger_level, -1);

	SoupSession *session = soup_session_new_with_options(SOUP_SESSION_MAX_CONNS,
		g_settings_get_uint(self->settings, "soup-max-conns"), SOUP_SESSION_USER_AGENT,
		"Harvest Almanac (tristan dot partin at gmail dot com)", SOUP_SESSION_ADD_FEATURE_BY_TYPE,
		SOUP_TYPE_CONTENT_SNIFFER, SOUP_SESSION_ADD_FEATURE, SOUP_SESSION_FEATURE(logger), NULL);

	self->client = harvest_api_client_v2_new(session, harvest_api_access_token, harvest_account_id);
}

static void
hal_application_startup(GApplication *self)
{
	g_resources_register(hal_get_resource());
	g_application_set_resource_base_path(self, "/io/partin/tristan/HarvestAlmanac");

	G_APPLICATION_CLASS(hal_application_parent_class)->startup(self);
}

static void
hal_application_finalize(GObject *obj)
{
	HalApplication *self = HAL_APPLICATION(obj);

	g_clear_object(&self->settings);

	G_OBJECT_CLASS(hal_application_parent_class)->finalize(obj);
}

static void
hal_application_class_init(HalApplicationClass *klass)
{
	GObjectClass *obj_class		 = G_OBJECT_CLASS(klass);
	GApplicationClass *app_class = G_APPLICATION_CLASS(klass);

	obj_class->finalize = hal_application_finalize;
	app_class->activate = hal_application_activate;
	app_class->startup  = hal_application_startup;
}

// clang-format off
static const GActionEntry app_entries[] = {
	{
		.name = "about", .activate = hal_application_about
	},
	{
		.name = "preferences", .activate = hal_application_preferences
	},
	{
		.name			= "time-entry-start",
		.activate		= hal_application_time_entry_start,
		.parameter_type = "t"
	},
	{
		.name = "time-entry-stop",
		.activate = hal_application_time_entry_stop,
		.parameter_type = "t"
	},
	{
		.name = "check-settings",
		.activate = hal_application_create_client
	}
};
// clang-format on

static void
hal_application_init(HalApplication *self)
{
	self->settings = g_settings_new("io.partin.tristan.HarvestAlmanac");

	g_action_map_add_action_entries(
		G_ACTION_MAP(self), app_entries, G_N_ELEMENTS(app_entries), self);
}

HalApplication *
hal_application_new(const char *id)
{
	return g_object_new(HAL_TYPE_APPLICATION, "application-id", id, NULL);
}
