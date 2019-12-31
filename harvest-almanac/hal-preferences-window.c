#include "config.h"

#define G_LOG_DOMAIN "HalPreferencesWindow"

#include <string.h>

#include <glib-object.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <handy.h>
#include <libsecret/secret.h>

#include "hal-preferences-window.h"

struct _HalPreferencesWindow
{
	HdyPreferencesWindow parent_instance;

	gboolean dirty : 1;
	GSettings *settings;
	// Cache the access token to prevent extra libsecret lookups
	char *cached_access_token;
	// Cancel libsecret operations on window close
	GCancellable *cancellable;
};

typedef struct HalPreferencesWindowPrivate
{
	GtkEntry *harvest_api_access_token_entry;
	GtkEntry *harvest_account_id_entry;
	GtkEntry *harvest_api_contact_email_entry;
	GtkSwitch *prefer_dark_theme_switch;
	GtkSpinButton *soup_max_connections_spin;
	GtkComboBoxText *soup_logger_level_combo;
	GtkButton *save_button;
	GtkButton *harvest_api_access_token_forget_button;
} HalPreferencesWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(
	HalPreferencesWindow, hal_preferences_window, HDY_TYPE_PREFERENCES_WINDOW)

enum HalPreferencesWindowProps
{
	PROP_0,
	PROP_SETTINGS,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

const SecretSchema *
hal_get_secret_schema(void)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
	// clang-format off
	static const SecretSchema hal_schema = {
		.name  = "io.partin.tristan.HarvestAlmanac",
		.flags = SECRET_SCHEMA_NONE,
		.attributes = {
			{ "contact-email", SECRET_SCHEMA_ATTRIBUTE_STRING },
			{ "account-id", SECRET_SCHEMA_ATTRIBUTE_STRING },
			{ "NULL", 0 },
		}
	};
	// clang-format on
#pragma GCC diagnostic pop

	return &hal_schema;
}

static void G_GNUC_UNUSED
hal_preferences_window_set_dirty(HalPreferencesWindow *self, gboolean dirty)
{
	HalPreferencesWindowPrivate *priv = hal_preferences_window_get_instance_private(self);

	self->dirty = dirty;
	gtk_widget_set_sensitive(GTK_WIDGET(priv->save_button), dirty);
}

static void
on_prefer_dark_theme_switch_notify_active(
	GObject *obj, G_GNUC_UNUSED GParamSpec *pspec, gpointer user_data)
{
	HalPreferencesWindow *self = HAL_PREFERENCES_WINDOW(user_data);

	if (gtk_switch_get_active(GTK_SWITCH(obj))
		!= g_settings_get_boolean(self->settings, "prefer-dark-theme")) {
		hal_preferences_window_set_dirty(self, TRUE);
	} else {
		hal_preferences_window_set_dirty(self, FALSE);
	}
}

static void
on_harvest_api_access_token_entry_changed(GtkEditable *widget, gpointer user_data)
{
	HalPreferencesWindow *self = HAL_PREFERENCES_WINDOW(user_data);

	if (!g_str_equal(gtk_entry_get_text(GTK_ENTRY(widget)),
			self->cached_access_token == NULL ? "" : self->cached_access_token)) {
		hal_preferences_window_set_dirty(self, TRUE);
	} else {
		hal_preferences_window_set_dirty(self, FALSE);
	}
}

static void
on_harvest_api_contact_email_entry_changed(GtkEditable *widget, gpointer user_data)
{
	HalPreferencesWindow *self = HAL_PREFERENCES_WINDOW(user_data);

	g_autofree char *contact_email
		= g_settings_get_string(self->settings, "harvest-api-contact-email");

	if (!g_str_equal(gtk_entry_get_text(GTK_ENTRY(widget)), contact_email)) {
		hal_preferences_window_set_dirty(self, TRUE);
	} else {
		hal_preferences_window_set_dirty(self, FALSE);
	}
}

static void
on_harvest_account_id_entry_changed(GtkEditable *widget, gpointer user_data)
{
	HalPreferencesWindow *self = HAL_PREFERENCES_WINDOW(user_data);

	g_autofree char *account_id = g_settings_get_string(self->settings, "harvest-account-id");

	if (!g_str_equal(gtk_entry_get_text(GTK_ENTRY(widget)), account_id)) {
		hal_preferences_window_set_dirty(self, TRUE);
	} else {
		hal_preferences_window_set_dirty(self, FALSE);
	}
}

static void
on_soup_logger_level_combo_changed(GtkComboBox *widget, gpointer user_data)
{
	HalPreferencesWindow *self = HAL_PREFERENCES_WINDOW(user_data);

	g_autoptr(GVariant) variant		= g_settings_get_value(self->settings, "soup-logger-level");
	unsigned char soup_logger_level = HAL_DEFAULT_SOUP_LOGGER_LEVEL;
	g_variant_get(variant, "y", &soup_logger_level);

	if ((unsigned char) gtk_combo_box_get_active(widget) != soup_logger_level) {
		hal_preferences_window_set_dirty(self, TRUE);
	} else {
		hal_preferences_window_set_dirty(self, FALSE);
	}
}

static void
on_soup_max_connections_spin_value_changed(GtkSpinButton *widget, gpointer user_data)
{
	HalPreferencesWindow *self = HAL_PREFERENCES_WINDOW(user_data);

	g_autoptr(GVariant) variant	 = g_settings_get_value(self->settings, "soup-max-connections");
	guint32 soup_max_connections = HAL_DEFAULT_SOUP_MAX_CONNECTIONS;
	g_variant_get(variant, "u", &soup_max_connections);

	if ((guint32) gtk_spin_button_get_value_as_int(widget) != soup_max_connections) {
		hal_preferences_window_set_dirty(self, TRUE);
	} else {
		hal_preferences_window_set_dirty(self, FALSE);
	}
}

static void
on_harvest_api_access_token_store(
	G_GNUC_UNUSED GObject *source, GAsyncResult *result, gpointer user_data)
{
	HalPreferencesWindow *self		  = HAL_PREFERENCES_WINDOW(user_data);
	HalPreferencesWindowPrivate *priv = hal_preferences_window_get_instance_private(self);

	g_autoptr(GError) err = NULL;

	secret_password_store_finish(result, &err);
	if (err != NULL) {
		g_critical("Failed storing Harvest API access token: %s", err->message);
		hal_preferences_window_set_dirty(self, TRUE);
	} else {
		hal_preferences_window_set_dirty(self, FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_api_access_token_forget_button), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_api_access_token_entry), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_account_id_entry), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_api_contact_email_entry), FALSE);

		GApplication *app = g_application_get_default();
		GActionMap *map	  = G_ACTION_MAP(app);
		GAction *action	  = g_action_map_lookup_action(map, "reconstruct-client");
		GVariant *data
			= g_variant_new_string(gtk_entry_get_text(priv->harvest_api_access_token_entry));
		g_action_activate(action, data);
	}
}

static void
on_harvest_api_access_token_lookup(
	G_GNUC_UNUSED GObject *source, GAsyncResult *result, gpointer user_data)
{
	HalPreferencesWindow *self		  = HAL_PREFERENCES_WINDOW(user_data);
	HalPreferencesWindowPrivate *priv = hal_preferences_window_get_instance_private(self);

	g_autoptr(GError) err = NULL;

	gchar *access_token = secret_password_lookup_finish(result, &err);
	if (err != NULL) {
		g_critical("Failed to lookup Harvest API access token: %s", access_token);
	} else if (access_token == NULL || strlen(access_token) == 0) {
		self->cached_access_token = NULL;
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_api_access_token_forget_button), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_api_access_token_entry), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_account_id_entry), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_api_contact_email_entry), TRUE);
	} else {
		self->cached_access_token = g_strdup(access_token);
		gtk_entry_set_text(priv->harvest_api_access_token_entry, access_token);
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_api_access_token_forget_button), TRUE);
		secret_password_free(access_token);
	}
}

static void
on_harvest_api_access_token_clear(
	G_GNUC_UNUSED GObject *source, GAsyncResult *result, gpointer user_data)
{
	HalPreferencesWindow *self		  = HAL_PREFERENCES_WINDOW(user_data);
	HalPreferencesWindowPrivate *priv = hal_preferences_window_get_instance_private(self);

	g_autoptr(GError) err = NULL;

	G_GNUC_UNUSED const gboolean cleared = secret_password_clear_finish(result, &err);
	if (err != NULL) {
		g_critical("Failed to clear Harvest API access token: %s", err->message);
	} else {
		self->cached_access_token = NULL;
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_api_access_token_forget_button), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_api_access_token_entry), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_account_id_entry), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(priv->harvest_api_contact_email_entry), TRUE);
	}
}

static void
on_save_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	HalPreferencesWindow *self		  = HAL_PREFERENCES_WINDOW(user_data);
	HalPreferencesWindowPrivate *priv = hal_preferences_window_get_instance_private(self);

	const char *account_id		= gtk_entry_get_text(priv->harvest_account_id_entry);
	const char *contact_email	= gtk_entry_get_text(priv->harvest_api_contact_email_entry);
	GVariant *soup_logger_level = g_variant_new(
		"y", gtk_combo_box_get_active(GTK_COMBO_BOX(priv->soup_logger_level_combo)));
	GVariant *soup_max_connections
		= g_variant_new("u", gtk_spin_button_get_value_as_int(priv->soup_max_connections_spin));

	// Update the cache
	self->cached_access_token = g_strdup(gtk_entry_get_text(priv->harvest_api_access_token_entry));
	g_settings_set_boolean(
		self->settings, "prefer-dark-theme", gtk_switch_get_active(priv->prefer_dark_theme_switch));
	g_settings_set_string(self->settings, "harvest-account-id", account_id);
	g_settings_set_string(self->settings, "harvest-api-contact-email", contact_email);
	g_settings_set_value(self->settings, "soup-logger-level", soup_logger_level);
	g_settings_set_value(self->settings, "soup-max-connections", soup_max_connections);
	secret_password_store(HAL_SECRET_SCHEMA, SECRET_COLLECTION_DEFAULT, "Harvest API Access Token",
		self->cached_access_token, self->cancellable, on_harvest_api_access_token_store, self,
		"account-id", account_id, "contact-email", contact_email, NULL);
}

static void
on_harvest_api_access_token_forget_button_clicked(
	G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	HalPreferencesWindow *self		  = HAL_PREFERENCES_WINDOW(user_data);
	HalPreferencesWindowPrivate *priv = hal_preferences_window_get_instance_private(self);

	g_autofree char *account_id = g_settings_get_string(self->settings, "harvest-account-id");
	g_autofree char *contact_email
		= g_settings_get_string(self->settings, "harvest-api-contact-email");

	secret_password_clear(HAL_SECRET_SCHEMA, self->cancellable, on_harvest_api_access_token_clear,
		self, "account-id", account_id, "contact-email", contact_email, NULL);

	g_settings_set_string(self->settings, "harvest-account-id", "");
	g_settings_set_string(self->settings, "harvest-api-contact-email", "");

	gtk_entry_set_text(priv->harvest_api_access_token_entry, "");
	gtk_entry_set_text(priv->harvest_account_id_entry, "");
	gtk_entry_set_text(priv->harvest_api_contact_email_entry, "");
}

static void
hal_preferences_window_constructed(GObject *obj)
{
	HalPreferencesWindow *self		  = HAL_PREFERENCES_WINDOW(obj);
	HalPreferencesWindowPrivate *priv = hal_preferences_window_get_instance_private(self);

	g_autofree char *account_id = g_settings_get_string(self->settings, "harvest-account-id");
	g_autofree char *contact_email
		= g_settings_get_string(self->settings, "harvest-api-contact-email");
	g_autoptr(GVariant) soup_max_connections_variant
		= g_settings_get_value(self->settings, "soup-max-connections");
	guint32 soup_max_connections = HAL_DEFAULT_SOUP_MAX_CONNECTIONS;
	g_variant_get(soup_max_connections_variant, "u", &soup_max_connections);
	g_autoptr(GVariant) soup_logger_level_variant
		= g_settings_get_value(self->settings, "soup-logger-level");
	unsigned char soup_logger_level = HAL_DEFAULT_SOUP_LOGGER_LEVEL;
	g_variant_get(soup_logger_level_variant, "y", &soup_logger_level);

	secret_password_lookup(HAL_SECRET_SCHEMA, self->cancellable, on_harvest_api_access_token_lookup,
		self, "account-id", account_id, "contact-email", contact_email, NULL);
	gtk_switch_set_active(priv->prefer_dark_theme_switch,
		g_settings_get_boolean(self->settings, "prefer-dark-theme"));
	gtk_entry_set_text(priv->harvest_account_id_entry, account_id);
	gtk_entry_set_text(priv->harvest_api_contact_email_entry, contact_email);
	gtk_spin_button_set_value(priv->soup_max_connections_spin, (gdouble) soup_max_connections);
	gtk_combo_box_set_active(
		GTK_COMBO_BOX(priv->soup_logger_level_combo), (gint) soup_logger_level);
}

static void
hal_preferences_window_finalize(GObject *obj)
{
	HalPreferencesWindow *self = HAL_PREFERENCES_WINDOW(obj);

	g_cancellable_cancel(self->cancellable);

	if (self->settings != NULL)
		g_object_unref(self->settings);
	g_free(self->cached_access_token);

	G_OBJECT_CLASS(hal_preferences_window_parent_class)->finalize(obj);
}

static void
hal_preferences_window_set_property(
	GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HalPreferencesWindow *self = HAL_PREFERENCES_WINDOW(obj);

	switch (prop_id) {
	case PROP_SETTINGS:
		if (self->settings != NULL)
			g_object_unref(self->settings);
		self->settings = g_value_dup_object(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
hal_preferences_window_class_init(HalPreferencesWindowClass *klass)
{
	GObjectClass *obj_class	  = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);

	obj_class->constructed	= hal_preferences_window_constructed;
	obj_class->finalize		= hal_preferences_window_finalize;
	obj_class->set_property = hal_preferences_window_set_property;

	obj_properties[PROP_SETTINGS]
		= g_param_spec_object("settings", _("Settings"), _("Application settings"), G_TYPE_SETTINGS,
			G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);

	gtk_widget_class_set_template_from_resource(
		wid_class, "/io/partin/tristan/HarvestAlmanac/ui/hal-preferences-window.ui");
	gtk_widget_class_bind_template_child_private(
		wid_class, HalPreferencesWindow, harvest_api_access_token_entry);
	gtk_widget_class_bind_template_child_private(
		wid_class, HalPreferencesWindow, harvest_api_access_token_forget_button);
	gtk_widget_class_bind_template_child_private(
		wid_class, HalPreferencesWindow, harvest_account_id_entry);
	gtk_widget_class_bind_template_child_private(
		wid_class, HalPreferencesWindow, harvest_api_contact_email_entry);
	gtk_widget_class_bind_template_child_private(
		wid_class, HalPreferencesWindow, prefer_dark_theme_switch);
	gtk_widget_class_bind_template_child_private(
		wid_class, HalPreferencesWindow, soup_max_connections_spin);
	gtk_widget_class_bind_template_child_private(
		wid_class, HalPreferencesWindow, soup_logger_level_combo);
	gtk_widget_class_bind_template_child_private(wid_class, HalPreferencesWindow, save_button);
	gtk_widget_class_bind_template_callback(wid_class, on_save_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_prefer_dark_theme_switch_notify_active);
	gtk_widget_class_bind_template_callback(wid_class, on_harvest_api_access_token_entry_changed);
	gtk_widget_class_bind_template_callback(wid_class, on_harvest_account_id_entry_changed);
	gtk_widget_class_bind_template_callback(wid_class, on_harvest_api_contact_email_entry_changed);
	gtk_widget_class_bind_template_callback(wid_class, on_soup_logger_level_combo_changed);
	gtk_widget_class_bind_template_callback(wid_class, on_soup_max_connections_spin_value_changed);
	gtk_widget_class_bind_template_callback(
		wid_class, on_harvest_api_access_token_forget_button_clicked);
}

static void
hal_preferences_window_init(HalPreferencesWindow *self)
{
	HalPreferencesWindowPrivate *priv = hal_preferences_window_get_instance_private(self);

	gtk_widget_init_template(GTK_WIDGET(self));

	HdyHeaderBar *header_bar = HDY_HEADER_BAR(gtk_window_get_titlebar(GTK_WINDOW(self)));
	hdy_header_bar_pack_start(header_bar, GTK_WIDGET(priv->save_button));
	gtk_widget_show_all(GTK_WIDGET(header_bar));

	self->cancellable = g_cancellable_new();
	hal_preferences_window_set_dirty(self, FALSE);
}

HalPreferencesWindow *
hal_preferences_window_new(GSettings *settings)
{
	return g_object_new(HAL_TYPE_PREFERENCES_WINDOW, "settings", settings, NULL);
}
