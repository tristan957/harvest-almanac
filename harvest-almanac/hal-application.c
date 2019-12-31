#include "config.h"

#define G_LOG_DOMAIN "HalApplication"

#include <string.h>

#include <glib-object.h>
#include <gtk/gtk.h>
#include <harvest-glib/harvest.h>
#include <libsecret/secret.h>
#include <libsoup/soup.h>

#include "hal-application.h"
#include "hal-context.h"
#include "hal-preferences-window.h"
#include "hal-resources.h"
#include "hal-time-entry.h"
#include "hal-window.h"

#define HAL_MAX_CONNS_PER_HOST 4

extern HalContext *CONTEXT;
extern HarvestApiClient *API_CLIENT;
extern HalTimeEntry *CURRENTLY_RUNNING_TIME_ENTRY;

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
validate_user(
	G_GNUC_UNUSED HarvestRequest *req, G_GNUC_UNUSED HarvestResponse *res, gpointer user_data)
{
	HalApplication *self		= HAL_APPLICATION(user_data);
	HalApplicationPrivate *priv = hal_application_get_instance_private(self);

	if (res->err == NULL) {
		hal_context_set_user(g_value_get_object(res->body));
		hal_window_show_content(priv->main_window);
	} else {
		g_critical("validate_user: %s", res->err->message);
		hal_window_hide_content(priv->main_window);
	}
}

static void
construct_client(HalApplication *self, const char *access_token, const char *account_id,
	const char *contact_email)
{
	g_autoptr(GVariant) max_connections_v
		= g_settings_get_value(self->settings, "soup-max-connections");
	guint32 max_connections = HAL_DEFAULT_SOUP_MAX_CONNECTIONS;
	g_variant_get(max_connections_v, "u", &max_connections);
	g_autoptr(GVariant) logger_level_v = g_settings_get_value(self->settings, "soup-logger-level");
	unsigned char logger_level		   = HAL_DEFAULT_SOUP_LOGGER_LEVEL;
	g_variant_get(logger_level_v, "y", &logger_level);

	g_autoptr(SoupLogger) logger = soup_logger_new(logger_level, -1);

	g_autofree char *user_agent = g_strdup_printf("Harvest Almanac (%s)", contact_email);

	g_autoptr(SoupSession) session = soup_session_new_with_options(SOUP_SESSION_MAX_CONNS,
		max_connections, SOUP_SESSION_USER_AGENT, user_agent, SOUP_SESSION_ADD_FEATURE_BY_TYPE,
		SOUP_TYPE_CONTENT_SNIFFER, SOUP_SESSION_ADD_FEATURE, SOUP_SESSION_FEATURE(logger), NULL);

	harvest_api_client_initialize(session, access_token, account_id);

	harvest_user_get_me_async(validate_user, self);
}

static void
hal_application_activate(GApplication *app)
{
	HalApplication *self		= HAL_APPLICATION(app);
	HalApplicationPrivate *priv = hal_application_get_instance_private(HAL_APPLICATION(self));

	if (priv->main_window == NULL) {
		priv->main_window = hal_window_new(app);

		g_autofree char *account_id = g_settings_get_string(self->settings, "harvest-account-id");
		g_autofree char *contact_email
			= g_settings_get_string(self->settings, "harvest-api-contact-email");

		g_autoptr(GError) err = NULL;

		// Synchronous since window hasn't been presented yet
		gchar *access_token = secret_password_lookup_sync(HAL_SECRET_SCHEMA, NULL, &err,
			"account-id", account_id, "contact-email", contact_email, NULL);
		if (err != NULL) {
			g_error("Failed to look up Harvest API access token: %s", err->message);
		} else if (access_token == NULL) {
			g_debug("Harvest API access token is <null>");
		} else {
			g_debug("Constructing Harvest API access token");
			construct_client(self, access_token, account_id, contact_email);
			secret_password_free(access_token);
		}

		g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme",
			g_settings_get_boolean(self->settings, "prefer-dark-theme"), NULL);

		g_autoptr(GtkCssProvider) css_provider = gtk_css_provider_new();
		gtk_css_provider_load_from_resource(
			css_provider, "/io/partin/tristan/HarvestAlmanac/ui/style.css");
		gtk_style_context_add_provider_for_screen(
			gtk_window_get_screen(GTK_WINDOW(priv->main_window)), GTK_STYLE_PROVIDER(css_provider),
			GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	}

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
	HalApplication *self = HAL_APPLICATION(data);

	HalPreferencesWindow *preferences_window = hal_preferences_window_new(self->settings);
	gtk_window_present(GTK_WINDOW(preferences_window));
}

static void
hal_application_time_entry_start(
	G_GNUC_UNUSED GSimpleAction *action, G_GNUC_UNUSED GVariant *param, gpointer data)
{
	g_autoptr(GNotification) notification = g_notification_new("Harvest Almanac");
	g_notification_set_body(notification, "Client -- Project timer started");
	g_notification_add_button(notification, "Stop Timer", "app.time-entry-stop");
	g_application_send_notification(G_APPLICATION(data), "time-entry", notification);
}

static void
hal_application_time_entry_stop(
	G_GNUC_UNUSED GSimpleAction *action, G_GNUC_UNUSED GVariant *param, gpointer data)
{
	hal_time_entry_stop(CURRENTLY_RUNNING_TIME_ENTRY);
	g_application_withdraw_notification(G_APPLICATION(data), "time-entry");
}

static void
hal_application_reconstruct_client(
	G_GNUC_UNUSED GSimpleAction *action, GVariant *param, gpointer data)
{
	HalApplication *self = HAL_APPLICATION(data);

	const char *harvest_api_access_token = g_variant_get_string(param, NULL);
	g_autofree char *harvest_api_contact_email
		= g_settings_get_string(self->settings, "harvest-api-contact-email");
	g_autofree char *harvest_account_id
		= g_settings_get_string(self->settings, "harvest-account-id");

	construct_client(self, harvest_api_access_token, harvest_account_id, harvest_api_contact_email);
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

	harvest_api_client_free();
	g_clear_object(&self->settings);
	g_clear_object(&CONTEXT);

	G_OBJECT_CLASS(hal_application_parent_class)->finalize(obj);
}

static void
hal_application_class_init(HalApplicationClass *klass)
{
	GObjectClass *obj_class		 = G_OBJECT_CLASS(klass);
	GApplicationClass *app_class = G_APPLICATION_CLASS(klass);

	obj_class->finalize = hal_application_finalize;
	app_class->activate = hal_application_activate;
	app_class->startup	= hal_application_startup;
}

// clang-format off
static const GActionEntry app_entries[] = {
	{
		.name	  = "about",
		.activate = hal_application_about
	},
	{
		.name	  = "preferences",
		.activate = hal_application_preferences
	},
	{
		.name			= "time-entry-start",
		.activate		= hal_application_time_entry_start
	},
	{
		.name			= "time-entry-stop",
		.activate		= hal_application_time_entry_stop
	},
	{
		.name	  		= "reconstruct-client",
		.activate 		= hal_application_reconstruct_client,
		.parameter_type = "s"
	}
};
// clang-format on

static void
hal_application_init(HalApplication *self)
{
	self->settings = g_settings_new("io.partin.tristan.HarvestAlmanac");

	g_action_map_add_action_entries(
		G_ACTION_MAP(self), app_entries, G_N_ELEMENTS(app_entries), self);

	hal_context_initialize();
}

HalApplication *
hal_application_new(const char *id)
{
	return g_object_new(HAL_TYPE_APPLICATION, "application-id", id, NULL);
}
