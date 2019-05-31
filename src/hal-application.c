#include <string.h>

#include <glib-object.h>
#include <gtk/gtk.h>

#include "hal-application.h"
#include "hal-config.h"
#include "hal-resources.h"
#include "hal-settings-dialog.h"
#include "hal-time-entry.h"
#include "hal-window.h"

struct _HalApplication
{
	GtkApplication parent_instance;
};

typedef struct HalApplicationPrivate
{
	HalWindow *main_window;

	GSettings *settings;
} HalApplicationPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalApplication, hal_application, GTK_TYPE_APPLICATION)

static void
hal_application_activate(GApplication *self)
{
	HalApplicationPrivate *priv = hal_application_get_instance_private(HAL_APPLICATION(self));

	if (priv->main_window == NULL) {
		priv->main_window = hal_window_new(self);
	}

	// g_autoptr(GNotification) notification = g_notification_new("Harvest Almanac");
	// g_notification_set_body(notification, "Welcome to Harvest Almanac");
	// g_application_send_notification(self, "harvest-almanac", notification);

	g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme",
				 g_settings_get_boolean(priv->settings, SETTINGS_PREFER_DARK_THEME), NULL);

	gtk_window_present(GTK_WINDOW(priv->main_window));
}

static void
hal_application_about(G_GNUC_UNUSED GSimpleAction *action, G_GNUC_UNUSED GVariant *param,
					  gpointer data)
{
	static const char *authors[] = {"Tristan Partin"};

	HalApplication *self		= HAL_APPLICATION(data);
	HalApplicationPrivate *priv = hal_application_get_instance_private(self);

	gtk_show_about_dialog(GTK_WINDOW(priv->main_window), "program-name", PACKAGE_NAME, "version",
						  PACKAGE_VERSION, "license-type", PACKAGE_LICENSE, "website",
						  PACKAGE_WEBSITE, "website-label", PACKAGE_WEBSITE_LABEL, "authors",
						  authors, "logo-icon-name", "trophy-gold", NULL);
}

static void
hal_application_settings(G_GNUC_UNUSED GSimpleAction *action, G_GNUC_UNUSED GVariant *param,
						 gpointer data)
{
	HalApplication *self		= HAL_APPLICATION(data);
	HalApplicationPrivate *priv = hal_application_get_instance_private(self);

	HalSettingsDialog *dialog =
		hal_settings_dialog_new(GTK_WINDOW(priv->main_window), priv->settings);
	const GtkResponseType response = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(GTK_WIDGET(dialog));

	if (response == GTK_RESPONSE_APPLY) {
		g_autofree gchar *harvest_api_key =
			g_settings_get_string(priv->settings, SETTINGS_HARVEST_API_KEY);
		GActionMap *map = G_ACTION_MAP(priv->main_window);
		if (strlen(harvest_api_key) != 0) {
			GAction *show_content = g_action_map_lookup_action(map, "show-content");
			g_action_activate(show_content, NULL);
		} else {
			GAction *show_content = g_action_map_lookup_action(map, "hide-content");
			g_action_activate(show_content, NULL);
		}
	}
}

static void
hal_application_time_entry_start(G_GNUC_UNUSED GSimpleAction *action, GVariant *param,
								 gpointer data)
{
	const guint64 address			  = g_variant_get_uint64(param);
	G_GNUC_UNUSED HalTimeEntry *entry = HAL_TIME_ENTRY((HalTimeEntry *) address);

	g_autoptr(GNotification) notification = g_notification_new("Harvest Almanac");
	g_notification_set_body(notification, "Client -- Project timer started");
	g_notification_add_button_with_target(notification, "Stop Timer", "app.time-entry-stop", "t",
										  address, NULL);
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
hal_application_startup(GApplication *self)
{
	g_resources_register(hal_get_resource());
	g_application_set_resource_base_path(self, "/io/partin/tristan/HarvestAlmanac");

	G_APPLICATION_CLASS(hal_application_parent_class)->startup(self);
}

static void
hal_application_finalize(GObject *obj)
{
	HalApplication *self		= HAL_APPLICATION(obj);
	HalApplicationPrivate *priv = hal_application_get_instance_private(self);

	g_clear_object(&priv->settings);

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

static const GActionEntry app_entries[] = {
	{.name = "about", .activate = hal_application_about},
	{.name = "settings", .activate = hal_application_settings},
	{.name			 = "time-entry-start",
	 .activate		 = hal_application_time_entry_start,
	 .parameter_type = "t"},
	{.name			 = "time-entry-stop",
	 .activate		 = hal_application_time_entry_stop,
	 .parameter_type = "t"}};

static void
hal_application_init(HalApplication *self)
{
	HalApplicationPrivate *priv = hal_application_get_instance_private(self);

	priv->settings = g_settings_new("io.partin.tristan.HarvestAlmanac");

	g_action_map_add_action_entries(G_ACTION_MAP(self), app_entries, G_N_ELEMENTS(app_entries),
									self);
}

HalApplication *
hal_application_new(const char *id)
{
	return g_object_new(HAL_TYPE_APPLICATION, "application-id", id, NULL);
}
