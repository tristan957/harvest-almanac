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

	/**
	 * user_validated keeps the view switchers from being visible when the user is not validated.
	 */
	gboolean user_validated : 1;
};

typedef struct HalWindowPrivate
{
	GtkStack *function_stack;
	HdySqueezer *function_squeezer;
	HdyViewSwitcherBar *function_switcher_bar;
	HdyViewSwitcher *function_wide_switcher;
	HdyViewSwitcher *function_narrow_switcher;
	GtkLabel *title_label;
	HalTimeTracker *time_tracker;
	HalProfile *profile;
} HalWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalWindow, hal_window, GTK_TYPE_APPLICATION_WINDOW)

void
hal_window_show_content(HalWindow *self)
{
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	/**
	 * There should probably be a check to make sure the access token is actually
	 * valid using the me endpoint.
	 */

	self->user_validated = TRUE;

	gtk_stack_set_visible_child_name(GTK_STACK(priv->profile), "profile");
}

void
hal_window_hide_content(HalWindow *self)
{
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	gtk_stack_set_visible_child_name(priv->function_stack, "profile");
	gtk_stack_set_visible_child_name(GTK_STACK(priv->profile), "harvest-api-access-token");
}

static void
hal_window_finalize(GObject *obj)
{
	G_OBJECT_CLASS(hal_window_parent_class)->finalize(obj);
}

static void
hal_window_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
	HalWindow *self		   = HAL_WINDOW(widget);
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	if (self->user_validated) {
		hdy_squeezer_set_child_enabled(priv->function_squeezer,
			GTK_WIDGET(priv->function_wide_switcher), allocation->width > 600);
		hdy_squeezer_set_child_enabled(priv->function_squeezer,
			GTK_WIDGET(priv->function_narrow_switcher), allocation->width > 400);
	} else {
		hdy_squeezer_set_child_enabled(
			priv->function_squeezer, GTK_WIDGET(priv->title_label), TRUE);
		hdy_squeezer_set_child_enabled(
			priv->function_squeezer, GTK_WIDGET(priv->function_wide_switcher), FALSE);
		hdy_squeezer_set_child_enabled(
			priv->function_squeezer, GTK_WIDGET(priv->function_narrow_switcher), FALSE);
	}

	GTK_WIDGET_CLASS(hal_window_parent_class)->size_allocate(widget, allocation);
}

static gboolean
hal_window_is_title_label_visible(
	G_GNUC_UNUSED GBinding *binding, const GValue *from_value, GValue *to_value, gpointer user_data)
{
	HalWindow *self		   = HAL_WINDOW(user_data);
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	g_value_set_boolean(
		to_value, g_value_get_object(from_value) == priv->title_label && self->user_validated);

	return TRUE;
}

static void
hal_window_class_init(HalWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class  = GTK_WIDGET_CLASS(klass);

	object_class->finalize = hal_window_finalize;

	wid_class->size_allocate = hal_window_size_allocate;

	gtk_widget_class_set_template_from_resource(
		wid_class, "/io/partin/tristan/HarvestAlmanac/ui/hal-window.ui");
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, function_stack);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, function_squeezer);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, function_switcher_bar);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, function_wide_switcher);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, function_narrow_switcher);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, title_label);
}

static void
hal_window_init(HalWindow *self)
{
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	gtk_widget_init_template(GTK_WIDGET(self));

	priv->profile	   = hal_profile_new();
	priv->time_tracker = hal_time_tracker_new();

	gtk_stack_add_titled(priv->function_stack, GTK_WIDGET(priv->profile), "profile", "Profile");
	gtk_stack_add_titled(
		priv->function_stack, GTK_WIDGET(priv->time_tracker), "time-tracker", "Time");
	gtk_container_child_set(GTK_CONTAINER(priv->function_stack), GTK_WIDGET(priv->profile),
		"icon-name", "user-info-symbolic", NULL);
	gtk_container_child_set(GTK_CONTAINER(priv->function_stack), GTK_WIDGET(priv->time_tracker),
		"icon-name", "document-open-recent-symbolic", NULL);

	self->user_validated = FALSE;

	g_object_bind_property_full(priv->function_squeezer, "visible-child",
		priv->function_switcher_bar, "reveal", G_BINDING_SYNC_CREATE,
		hal_window_is_title_label_visible, NULL, self, NULL);
}

HalWindow *
hal_window_new(GApplication *app)
{
	return g_object_new(HAL_TYPE_WINDOW, "application", app, NULL);
}
