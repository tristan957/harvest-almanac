#include <glib-object.h>
#include <gtk/gtk.h>

#include "hal-time-entry.h"
#include "hal-time-tracker.h"

struct _HalTimeTracker
{
	GtkBox parent_instance;
};

typedef struct HalTimeTrackerPrivate
{
	GtkButton *monday_button;
	GtkButton *tuesday_button;
	GtkButton *wednesday_button;
	GtkButton *thursday_button;
	GtkButton *friday_button;
	GtkButton *saturday_button;
	GtkButton *sunday_button;
	GtkLabel *monday_time;
	GtkLabel *tuesday_time;
	GtkLabel *wednesday_time;
	GtkLabel *thursday_time;
	GtkLabel *friday_time;
	GtkLabel *saturday_time;
	GtkLabel *sunday_time;
	GtkListBox *monday_time_entries;
	GtkListBox *tuesday_time_entries;
	GtkListBox *wednesday_time_entries;
	GtkListBox *thursday_time_entries;
	GtkListBox *friday_time_entries;
	GtkListBox *saturday_time_entries;
	GtkListBox *sunday_time_entries;
} HalTimeTrackerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalTimeTracker, hal_time_tracker, GTK_TYPE_BOX)

enum HalTimeTrackerProps
{
	PROP_0,
	PROP_CLIENT,
	PROP_USER,
	N_PROPS
};

G_GNUC_UNUSED static GParamSpec *obj_properties[N_PROPS];

static void
hal_time_tracker_clear_entries(HalTimeTracker *self)
{
	G_GNUC_UNUSED HalTimeTrackerPrivate *priv = hal_time_tracker_get_instance_private(self);
}

static void
on_monday_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	gtk_stack_set_visible_child_name(GTK_STACK(user_data), "monday");
}

static void
on_tuesday_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	gtk_stack_set_visible_child_name(GTK_STACK(user_data), "tuesday");
}

static void
on_wednesday_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	gtk_stack_set_visible_child_name(GTK_STACK(user_data), "wednesday");
}

static void
on_thursday_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	gtk_stack_set_visible_child_name(GTK_STACK(user_data), "thursday");
}

static void
on_friday_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	gtk_stack_set_visible_child_name(GTK_STACK(user_data), "friday");
}

static void
on_saturday_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	gtk_stack_set_visible_child_name(GTK_STACK(user_data), "saturday");
}

static void
on_sunday_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	gtk_stack_set_visible_child_name(GTK_STACK(user_data), "sunday");
}

static void
hal_time_tracker_finalize(GObject *self)
{
	hal_time_tracker_clear_entries(HAL_TIME_TRACKER(self));

	G_OBJECT_CLASS(hal_time_tracker_parent_class)->finalize(self);
}

static void
hal_time_tracker_get_property(
	GObject *obj, guint prop_id, G_GNUC_UNUSED GValue *val, GParamSpec *pspec)
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
hal_time_tracker_set_property(
	GObject *obj, guint prop_id, G_GNUC_UNUSED const GValue *val, GParamSpec *pspec)
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
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, monday_button);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, tuesday_button);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, wednesday_button);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, thursday_button);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, friday_button);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, saturday_button);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, sunday_button);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, monday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, tuesday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, wednesday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, thursday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, friday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, saturday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, sunday_time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, monday_time_entries);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, tuesday_time_entries);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, wednesday_time_entries);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, thursday_time_entries);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, friday_time_entries);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, saturday_time_entries);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeTracker, sunday_time_entries);
	gtk_widget_class_bind_template_callback(wid_class, on_monday_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_tuesday_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_wednesday_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_thursday_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_friday_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_saturday_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_sunday_button_clicked);
}

static void
hal_time_tracker_init(HalTimeTracker *self)
{
	HalTimeTrackerPrivate *priv = hal_time_tracker_get_instance_private(self);

	gtk_widget_init_template(GTK_WIDGET(self));

	gtk_list_box_insert(priv->monday_time_entries, GTK_WIDGET(hal_time_entry_new()), -1);
	gtk_list_box_insert(priv->tuesday_time_entries, GTK_WIDGET(hal_time_entry_new()), -1);
	gtk_list_box_insert(priv->wednesday_time_entries, GTK_WIDGET(hal_time_entry_new()), -1);
	gtk_list_box_insert(priv->thursday_time_entries, GTK_WIDGET(hal_time_entry_new()), -1);
	gtk_list_box_insert(priv->friday_time_entries, GTK_WIDGET(hal_time_entry_new()), -1);
	gtk_list_box_insert(priv->saturday_time_entries, GTK_WIDGET(hal_time_entry_new()), -1);
	gtk_list_box_insert(priv->sunday_time_entries, GTK_WIDGET(hal_time_entry_new()), -1);
}

HalTimeTracker *
hal_time_tracker_new()
{
	return g_object_new(HAL_TYPE_TIME_TRACKER, NULL);
}
