#include <glib-object.h>
#include <glib/gi18n.h>
#include <json-glib/json-glib.h>

#include "harvest-client.h"
#include "harvest-invoice.h"
#include "harvest-project.h"
#include "harvest-task-assignment.h"
#include "harvest-task.h"
#include "harvest-time-entry.h"
#include "harvest-user-assignment.h"
#include "harvest-user.h"

struct _HarvestTimeEntry
{
	GObject parent_instance;

	int id;
	char *spent_date;
	HarvestUser *user;
	HarvestUserAssignment *user_assignment;
	HarvestClient *client;
	HarvestProject *project;
	HarvestTask *task;
	HarvestTaskAssignment *task_assingment;
	HarvestInvoice *invoice;
	double hours;
	char *notes;
	gboolean is_locked;
	char *locked_reason;
	gboolean is_closed;
	gboolean is_billed;
	char *timer_started_at;
	char *started_time;
	char *ended_time;
	gboolean is_running;
	gboolean billable;
	gboolean budgeted;
	double billable_rate;
	double cost_rate;
	char *created_at;
	char *updated_at;
};

G_DEFINE_TYPE(HarvestTimeEntry, harvest_time_entry, G_TYPE_OBJECT)

enum HarvestTimeEntryProps
{
	PROP_0,
	PROP_ID,
	PROP_SPENT_DATA,
	PROP_USER,
	PROP_USER_ASSIGNMENT,
	PROP_CLIENT,
	PROP_PROJECT,
	PROP_TASK,
	PROP_TASK_ASSIGNMEMT,
	PROP_EXTERNAL_REFERENCE,
	PROP_INVOICE,
	PROP_HOURS,
	PROP_NOTES,
	PROP_IS_LOCKED,
	PROP_LOCKED_REASON,
	PROP_IS_CLOSED,
	PROP_IS_BILLED,
	PROP_TIMER_STARTED_AT,
	PROP_STARTED_TIME,
	PROP_ENDED_TIME,
	PROP_IS_RUNNING,
	PROP_BILLABLE,
	PROP_BUDGETED,
	PROP_BILLABLE_RATE,
	PROP_COST_RATE,
	PROP_CREATED_AT,
	PROP_UPDATED_AT,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_time_entry_finalize(GObject *obj)
{
	G_GNUC_UNUSED HarvestTimeEntry *self = HARVEST_TIME_ENTRY(obj);

	G_OBJECT_CLASS(harvest_time_entry_parent_class)->finalize(obj);
}

static void
harvest_time_entry_get_property(
	GObject *obj, guint prop_id, G_GNUC_UNUSED GValue *val, GParamSpec *pspec)
{
	G_GNUC_UNUSED HarvestTimeEntry *self = HARVEST_TIME_ENTRY(obj);

	switch (prop_id) {
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_time_entry_set_property(
	GObject *obj, guint prop_id, G_GNUC_UNUSED const GValue *val, GParamSpec *pspec)
{
	G_GNUC_UNUSED HarvestTimeEntry *self = HARVEST_TIME_ENTRY(obj);

	switch (prop_id) {
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_time_entry_class_init(HarvestTimeEntryClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_time_entry_finalize;
	obj_class->get_property = harvest_time_entry_get_property;
	obj_class->set_property = harvest_time_entry_set_property;

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_time_entry_init(G_GNUC_UNUSED HarvestTimeEntry *self)
{}
