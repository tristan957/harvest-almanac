#include <glib-object.h>
#include <gtk/gtk.h>

#include "hal-config.h"
#include "hal-profile.h"
#include "hal-time-tracker.h"
#include "hal-window.h"

struct _HalWindow
{
	GtkApplicationWindow parent_instance;
};

typedef struct HalWindowPrivate
{
	GtkStack *function_stack;
	GtkStackSwitcher *function_switcher;
	HalTimeTracker *time_tracker;
	HalProfile *profile;
} HalWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalWindow, hal_window, GTK_TYPE_APPLICATION_WINDOW)

static void
hal_window_show_content(G_GNUC_UNUSED GSimpleAction *action, G_GNUC_UNUSED GVariant *param,
						gpointer data)
{
	HalWindow *self		   = HAL_WINDOW(data);
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	/**
	 * There should probably be a check to make sure the API key is actually
	 * valid using the me endpoint.
	 */

	gtk_stack_set_visible_child_name(GTK_STACK(priv->profile), "profile");
	gtk_widget_set_sensitive(GTK_WIDGET(priv->function_switcher), TRUE);
}

static void
hal_window_hide_content(G_GNUC_UNUSED GSimpleAction *action, G_GNUC_UNUSED GVariant *param,
						gpointer data)
{
	HalWindow *self		   = HAL_WINDOW(data);
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	gtk_stack_set_visible_child_name(priv->function_stack, "profile");
	gtk_stack_set_visible_child_name(GTK_STACK(priv->profile), "harvest-api-key");
	gtk_widget_set_sensitive(GTK_WIDGET(priv->function_switcher), FALSE);
}

static void
hal_window_finalize(GObject *obj)
{
	G_OBJECT_CLASS(hal_window_parent_class)->finalize(obj);
}

static void
hal_window_class_init(HalWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class  = GTK_WIDGET_CLASS(klass);

	object_class->finalize = hal_window_finalize;

	gtk_widget_class_set_template_from_resource(
		wid_class, "/io/partin/tristan/HarvestAlmanac/ui/hal-window.ui");
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, function_stack);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, function_switcher);
}

static const GActionEntry win_entries[] = {
	{.name = "show-content", .activate = hal_window_show_content},
	{.name = "hide-content", .activate = hal_window_hide_content}};

static void
hal_window_init(HalWindow *self)
{
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	g_action_map_add_action_entries(G_ACTION_MAP(self), win_entries, G_N_ELEMENTS(win_entries),
									self);

	gtk_widget_init_template(GTK_WIDGET(self));

	priv->profile	  = hal_profile_new();
	priv->time_tracker = hal_time_tracker_new();

	gtk_stack_add_titled(priv->function_stack, GTK_WIDGET(priv->profile), "profile", "Profile");
	gtk_stack_add_titled(priv->function_stack, GTK_WIDGET(priv->time_tracker), "time-tracker",
						 "Time");

	g_autoptr(GSettings) settings	 = g_settings_new("io.partin.tristan.HarvestAlmanac");
	g_autofree gchar *harvest_api_key = g_settings_get_string(settings, SETTINGS_HARVEST_API_KEY);
	GActionMap *map					  = G_ACTION_MAP(self);
	if (strlen(harvest_api_key) != 0) {
		GAction *show_content = g_action_map_lookup_action(map, "show-content");
		g_action_activate(show_content, NULL);
	} else {
		GAction *show_content = g_action_map_lookup_action(map, "hide-content");
		g_action_activate(show_content, NULL);
	}
}

HalWindow *
hal_window_new(GApplication *app)
{
	return g_object_new(HAL_TYPE_WINDOW, "application", app, NULL);
}
