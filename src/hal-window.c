#include <gtk/gtk.h>

#include "hal-window.h"

struct _HalWindow
{
	GtkApplicationWindow parent_instance;
};

typedef struct HalWindowPrivate
{
	GtkLabel *label;
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
		wid_class, "/ht/sr/git/tristan957/harvest-almanac/ui/hal-window.ui");
}

static void
hal_window_init(HalWindow *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}

HalWindow *
hal_window_new(GApplication *app)
{
	return g_object_new(HAL_TYPE_WINDOW, "application", app, NULL);
}
