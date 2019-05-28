#include <glib-object.h>
#include <gtk/gtk.h>

#include "hal-time-tracker.h"
#include "hal-window.h"

struct _HalWindow
{
	GtkApplicationWindow parent_instance;
};

typedef struct HalWindowPrivate
{
	GtkStack *header_stack;
	GtkStack *content_stack;
	GtkStack *function_stack;
	HalTimeTracker *time_tracker;
} HalWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalWindow, hal_window, GTK_TYPE_APPLICATION_WINDOW)

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
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, header_stack);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, content_stack);
	gtk_widget_class_bind_template_child_private(wid_class, HalWindow, function_stack);
}

static void
hal_window_init(HalWindow *self)
{
	HalWindowPrivate *priv = hal_window_get_instance_private(self);

	gtk_widget_init_template(GTK_WIDGET(self));

	priv->time_tracker = hal_time_tracker_new();
	gtk_stack_add_titled(priv->function_stack, GTK_WIDGET(priv->time_tracker), "time-tracker",
						 "Time");
	gtk_stack_set_visible_child_name(priv->content_stack, "function-stack");
	gtk_stack_set_visible_child_name(priv->header_stack, "function-switcher");
}

HalWindow *
hal_window_new(GApplication *app)
{
	return g_object_new(HAL_TYPE_WINDOW, "application", app, NULL);
}
