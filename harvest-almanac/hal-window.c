#include "config.h"

#define G_LOG_DOMAIN "HalWindow"

#include <glib-object.h>
#include <gtk/gtk.h>
#include <handy.h>

#include "hal-profile.h"
#include "hal-time-tracker.h"
#include "hal-window.h"

struct _HalWindow
{
	GtkApplicationWindow parent_instance;
};

typedef struct HalWindowPrivate
{
	HdyHeaderGroup *header_group;
	HdyLeaflet *content_leaflet;
	HdyLeaflet *header_leaflet;
	GtkHeaderBar *header_bar;
	GtkHeaderBar *sub_header_bar;
	GtkStackSidebar *stack_sidebar;
	GtkStack *stack;
	GtkButton *back_button;
	HalTimeTracker *time_tracker;
	HalProfile *profile;
} HalWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalWindow, hal_window, GTK_TYPE_APPLICATION_WINDOW)

static void
update_header_bar_title(HalWindow *self)
{
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	GtkWidget *child = gtk_stack_get_visible_child(priv->stack);
	GValue title	 = G_VALUE_INIT;
	g_value_init(&title, G_TYPE_STRING);
	gtk_container_child_get_property(GTK_CONTAINER(priv->stack), child, "title", &title);
	gtk_header_bar_set_title(priv->sub_header_bar, g_value_get_string(&title));
}

static void
update_focus(HalWindow *self)
{
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	GtkWidget *header_child = hdy_leaflet_get_visible_child(priv->header_leaflet);
	const HdyFold fold		= hdy_leaflet_get_fold(priv->header_leaflet);

	g_assert(header_child == NULL || GTK_IS_HEADER_BAR(header_child));

	hdy_header_group_set_focus(
		priv->header_group, fold == HDY_FOLD_FOLDED ? GTK_HEADER_BAR(header_child) : NULL);
}

static void
stack_notify_visible_child_cb(
	G_GNUC_UNUSED GObject *obj, G_GNUC_UNUSED GParamSpec *pspec, gpointer user_data)
{
	HalWindow *self = HAL_WINDOW(user_data);

	update_header_bar_title(self);
}

static void
header_leaflet_notify_visible_child_cb(
	G_GNUC_UNUSED GObject *obj, G_GNUC_UNUSED GParamSpec *pspec, gpointer user_data)
{
	HalWindow *self = HAL_WINDOW(user_data);

	update_focus(self);
}

static void
header_leaflet_notify_fold_cb(
	G_GNUC_UNUSED GObject *obj, G_GNUC_UNUSED GParamSpec *pspec, gpointer user_data)
{
	HalWindow *self = HAL_WINDOW(user_data);

	update_focus(self);
}

static void
back_button_clicked_cb(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	HalWindow *self		   = HAL_WINDOW(user_data);
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	hdy_leaflet_set_visible_child_name(priv->content_leaflet, "sidebar");
}

void
hal_window_show_content(HalWindow *self)
{
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	gtk_stack_set_visible_child_name(GTK_STACK(priv->profile), "profile");
}

void
hal_window_hide_content(HalWindow *self)
{
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	gtk_stack_set_visible_child_name(priv->stack, "profile");
	gtk_stack_set_visible_child_name(GTK_STACK(priv->profile), "harvest-api-access-token");
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
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, header_group);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, content_leaflet);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, header_leaflet);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, header_bar);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, sub_header_bar);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, stack_sidebar);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, stack);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, back_button);
	gtk_widget_class_bind_template_callback(wid_class, header_leaflet_notify_fold_cb);
	gtk_widget_class_bind_template_callback(wid_class, header_leaflet_notify_visible_child_cb);
	gtk_widget_class_bind_template_callback(wid_class, stack_notify_visible_child_cb);
	gtk_widget_class_bind_template_callback(wid_class, back_button_clicked_cb);
}

static void
hal_window_init(HalWindow *self)
{
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	gtk_widget_init_template(GTK_WIDGET(self));

	priv->profile	   = hal_profile_new();
	priv->time_tracker = hal_time_tracker_new();

	gtk_stack_add_titled(priv->stack, GTK_WIDGET(priv->profile), "profile", "Profile");
	gtk_stack_add_titled(priv->stack, GTK_WIDGET(priv->time_tracker), "time-tracker", "Time");

	update_header_bar_title(self);

	hdy_leaflet_set_visible_child_name(priv->content_leaflet, "content");
}

HalWindow *
hal_window_new(GApplication *app)
{
	return g_object_new(HAL_TYPE_WINDOW, "application", app, NULL);
}
