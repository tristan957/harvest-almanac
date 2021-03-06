#include "config.h"

#include <glib-object.h>
#include <gtk/gtk.h>

#include "hal-time-entry.h"

// The time entry which the notification alludes to
HalTimeEntry *CURRENTLY_RUNNING_TIME_ENTRY;

struct _HalTimeEntry
{
	GtkGrid parent_instance;

	GTimer *timer;
	GCancellable *cancellable;
};

typedef struct HalTimeEntryPrivate
{
	GtkRevealer *billable_revealer;
	GtkLabel *time;
	GtkLabel *client;
	GtkLabel *project;
	GtkLabel *thursday_time;
	GtkLabel *description;
	GtkStack *status_stack;
	GtkButton *start_button;
	GtkButton *stop_button;
} HalTimeEntryPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalTimeEntry, hal_time_entry, GTK_TYPE_GRID)

enum HalTimeEntryProps
{
	PROP_0,
	PROP_ENTRY,
	N_PROPS
};

G_GNUC_UNUSED static GParamSpec *obj_properties[N_PROPS];

static gboolean
update_time_label(gpointer user_data)
{
	HalTimeEntry *self		  = HAL_TIME_ENTRY(user_data);
	HalTimeEntryPrivate *priv = hal_time_entry_get_instance_private(self);

	if (self->cancellable == NULL || g_cancellable_is_cancelled(self->cancellable)) {
		return G_SOURCE_REMOVE;
	}

	const int elapsed	   = g_timer_elapsed(self->timer, NULL);
	g_autofree char *label = g_strdup_printf("%d:%02d", elapsed / 3600, (elapsed % 3600) / 60);
	gtk_label_set_text(priv->time, label);

	return G_SOURCE_CONTINUE;
}

static void
on_status_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	HalTimeEntry *self		  = HAL_TIME_ENTRY(user_data);
	HalTimeEntryPrivate *priv = hal_time_entry_get_instance_private(self);

	if (!g_timer_is_active(self->timer)) {
		CURRENTLY_RUNNING_TIME_ENTRY = self;
		g_timer_continue(self->timer);
		gtk_stack_set_visible_child_name(priv->status_stack, "stop");
		g_cancellable_reset(self->cancellable);
		g_timeout_add(500, update_time_label, self);
	} else {
		CURRENTLY_RUNNING_TIME_ENTRY = NULL;
		hal_time_entry_stop(self);
	}
}

static void
hal_time_entry_finalize(GObject *obj)
{
	HalTimeEntry *self = HAL_TIME_ENTRY(obj);

	if (!g_cancellable_is_cancelled(self->cancellable)) {
		g_cancellable_cancel(self->cancellable);
	}

	g_object_unref(self->cancellable);
	g_timer_destroy(self->timer);

	G_OBJECT_CLASS(hal_time_entry_parent_class)->finalize(obj);
}

static void
hal_time_entry_get_property(
	GObject *obj, guint prop_id, G_GNUC_UNUSED GValue *val, GParamSpec *pspec)
{
	switch (prop_id) {
	case PROP_ENTRY:
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
hal_time_entry_set_property(
	GObject *obj, guint prop_id, G_GNUC_UNUSED const GValue *val, GParamSpec *pspec)
{
	switch (prop_id) {
	case PROP_ENTRY:
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
hal_time_entry_class_init(HalTimeEntryClass *klass)
{
	GObjectClass *obj_class	  = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);

	obj_class->finalize		= hal_time_entry_finalize;
	obj_class->get_property = hal_time_entry_get_property;
	obj_class->set_property = hal_time_entry_set_property;

	// TODO: actually create GParamSpec

	gtk_widget_class_set_template_from_resource(
		wid_class, "/io/partin/tristan/HarvestAlmanac/ui/hal-time-entry.ui");
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeEntry, billable_revealer);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeEntry, time);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeEntry, client);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeEntry, project);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeEntry, description);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeEntry, status_stack);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeEntry, start_button);
	gtk_widget_class_bind_template_child_private(wid_class, HalTimeEntry, stop_button);
	gtk_widget_class_bind_template_callback(wid_class, on_status_button_clicked);
}

static void
hal_time_entry_init(HalTimeEntry *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));

	self->timer = g_timer_new();
	g_timer_stop(self->timer);
	self->cancellable = g_cancellable_new();
}

HalTimeEntry *
hal_time_entry_new()
{
	return g_object_new(HAL_TYPE_TIME_ENTRY, NULL);
}

void
hal_time_entry_stop(HalTimeEntry *self)
{
	HalTimeEntryPrivate *priv = hal_time_entry_get_instance_private(self);

	if (g_timer_is_active(self->timer) && !g_cancellable_is_cancelled(self->cancellable)) {
		g_timer_stop(self->timer);
		g_cancellable_cancel(self->cancellable);
		gtk_stack_set_visible_child_name(priv->status_stack, "start");
	}
}
