#include <glib-object.h>
#include <gtk/gtk.h>

#include "hal-time-tracker.h"

struct _HalTimeTracker
{
	GtkBox parent_instance;
};

typedef struct HalTimeTrackerPrivate
{
	GtkLabel *monday_time;
	GtkLabel *tuesday_time;
	GtkLabel *wednesday_time;
	GtkLabel *thursday_time;
	GtkLabel *friday_time;
	GtkLabel *saturday_time;
	GtkLabel *sunday_time;
} HalTimeTrackerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalTimeTracker, hal_time_tracker, GTK_TYPE_BOX)

enum HalTimeTrackerProps
{
	PROP_CLIENT = 1,
	PROP_USER,
	N_PROPS
};

G_GNUC_UNUSED static GParamSpec *obj_properties[N_PROPS];

static void
hal_time_tracker_finalize(GObject *self)
{
	G_OBJECT_CLASS(hal_time_tracker_parent_class)->finalize(self);
}

static void
hal_time_tracker_get_property(GObject *obj, guint prop_id, G_GNUC_UNUSED GValue *val,
							  GParamSpec *pspec)
{
	switch (prop_id) {
	case PROP_CLIENT:
		break;
	case PROP_USER:
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
hal_time_tracker_set_property(GObject *obj, guint prop_id, G_GNUC_UNUSED const GValue *val,
							  GParamSpec *pspec)
{
	switch (prop_id) {
	case PROP_CLIENT:
		break;
	case PROP_USER:
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
hal_time_tracker_class_init(HalTimeTrackerClass *klass)
{
	GObjectClass *obj_class   = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);

	obj_class->finalize		= hal_time_tracker_finalize;
	obj_class->get_property = hal_time_tracker_get_property;
	obj_class->set_property = hal_time_tracker_set_property;

	gtk_widget_class_set_template_from_resource(
		wid_class, "/io/partin/tristan/HarvestAlmanac/ui/hal-time-tracker.ui");
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, monday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, tuesday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, wednesday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, thursday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, friday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, saturday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, sunday_time);
}

static void
hal_time_tracker_init(HalTimeTracker *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}

HalTimeTracker *
hal_time_tracker_new()
{
	return g_object_new(HAL_TYPE_TIME_TRACKER, NULL);
}
