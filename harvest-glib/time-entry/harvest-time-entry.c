#include "harvest-glib/config.h"

#include <float.h>
#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n-lib.h>
#include <json-glib/json-glib.h>

#include "harvest-glib/client/harvest-client.h"
#include "harvest-glib/common/harvest-common.h"
#include "harvest-glib/invoice/harvest-invoice.h"
#include "harvest-glib/project/harvest-project.h"
#include "harvest-glib/project/harvest-user-assignment.h"
#include "harvest-glib/task/harvest-task-assignment.h"
#include "harvest-glib/task/harvest-task.h"
#include "harvest-glib/time-entry/harvest-time-entry.h"
#include "harvest-glib/user/harvest-user.h"

struct _HarvestTimeEntry
{
	GObject parent_instance;

	int id;
	GDateTime *spent_date;
	HarvestUser *user;
	HarvestUserAssignment *user_assignment;
	HarvestClient *client;
	HarvestProject *project;
	HarvestTask *task;
	HarvestTaskAssignment *task_assingment;
	HarvestInvoice *invoice;
	double hours;
	char *notes;
	gboolean is_locked : 1;
	char *locked_reason;
	gboolean is_closed : 1;
	gboolean is_billed : 1;
	GDateTime *timer_started_at;
	GDateTime *started_time;
	GDateTime *ended_time;
	gboolean is_running : 1;
	gboolean billable : 1;
	gboolean budgeted : 1;
	double billable_rate;
	double cost_rate;
	GDateTime *created_at;
	GDateTime *updated_at;
};

static void harvest_time_entry_json_serializable_init(JsonSerializableIface *iface);

G_DEFINE_TYPE_WITH_CODE(HarvestTimeEntry, harvest_time_entry, G_TYPE_OBJECT,
	G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, harvest_time_entry_json_serializable_init))

enum HarvestTimeEntryProps
{
	PROP_0,
	PROP_ID,
	PROP_SPENT_DATE,
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

static gboolean
harvest_time_entry_deserialize_property(JsonSerializable *serializable, const gchar *prop_name,
	GValue *val, GParamSpec *pspec, JsonNode *prop_node)
{
	if (g_strcmp0(prop_name, "spent_date") == 0) {
		const GDateTime *dt
			= g_date_time_new_from_abbreviated_date(json_node_get_string(prop_node));
		g_value_set_boxed(val, dt);

		return TRUE;
	} else if (g_strcmp0(prop_name, "created_at") == 0 || g_strcmp0(prop_name, "updated_at") == 0
			   || g_strcmp0(prop_name, "timer_started_at") == 0
			   || g_strcmp0(prop_name, "started_time") == 0
			   || g_strcmp0(prop_name, "ended_time") == 0) {
		const GDateTime *dt = g_date_time_new_from_iso8601(json_node_get_string(prop_node), NULL);
		g_value_set_boxed(val, dt);

		return TRUE;
	} else if (g_strcmp0(prop_name, "user") == 0) {
		GObject *obj = json_gobject_deserialize(HARVEST_TYPE_USER, prop_node);
		g_value_set_object(val, obj);

		return TRUE;
	} else if (g_strcmp0(prop_name, "user_assignment") == 0) {
		GObject *obj = json_gobject_deserialize(HARVEST_TYPE_USER_ASSIGNMENT, prop_node);
		g_value_set_object(val, obj);

		return TRUE;
	} else if (g_strcmp0(prop_name, "client") == 0) {
		GObject *obj = json_gobject_deserialize(HARVEST_TYPE_CLIENT, prop_node);
		g_value_set_object(val, obj);

		return TRUE;
	} else if (g_strcmp0(prop_name, "project") == 0) {
		GObject *obj = json_gobject_deserialize(HARVEST_TYPE_PROJECT, prop_node);
		g_value_set_object(val, obj);

		return TRUE;
	} else if (g_strcmp0(prop_name, "task") == 0) {
		GObject *obj = json_gobject_deserialize(HARVEST_TYPE_TASK, prop_node);
		g_value_set_object(val, obj);

		return TRUE;
	} else if (g_strcmp0(prop_name, "task_assignment") == 0) {
		GObject *obj = json_gobject_deserialize(HARVEST_TYPE_TASK_ASSIGNMENT, prop_node);
		g_value_set_object(val, obj);

		return TRUE;
	} else if (g_strcmp0(prop_name, "invoice") == 0) {
		GObject *obj = json_gobject_deserialize(HARVEST_TYPE_INVOICE, prop_node);
		g_value_set_object(val, obj);

		return TRUE;
	}

	return json_serializable_default_deserialize_property(
		serializable, prop_name, val, pspec, prop_node);
}

static void
harvest_time_entry_json_serializable_init(JsonSerializableIface *iface)
{
	iface->deserialize_property = harvest_time_entry_deserialize_property;
}

static void
harvest_time_entry_finalize(GObject *obj)
{
	HarvestTimeEntry *self = HARVEST_TIME_ENTRY(obj);

	if (self->spent_date != NULL)
		g_date_time_unref(self->spent_date);
	if (self->user != NULL)
		g_object_unref(self->user);
	if (self->user_assignment != NULL)
		g_object_unref(self->user_assignment);
	if (self->client != NULL)
		g_object_unref(self->client);
	if (self->project != NULL)
		g_object_unref(self->project);
	if (self->task != NULL)
		g_object_unref(self->task);
	if (self->task_assingment != NULL)
		g_object_unref(self->task_assingment);
	if (self->invoice != NULL)
		g_object_unref(self->invoice);
	g_free(self->notes);
	g_free(self->locked_reason);
	if (self->timer_started_at != NULL)
		g_date_time_unref(self->timer_started_at);
	if (self->started_time != NULL)
		g_date_time_unref(self->started_time);
	if (self->ended_time != NULL)
		g_date_time_unref(self->ended_time);
	if (self->created_at != NULL)
		g_date_time_unref(self->created_at);
	if (self->updated_at != NULL)
		g_date_time_unref(self->updated_at);

	G_OBJECT_CLASS(harvest_time_entry_parent_class)->finalize(obj);
}

static void
harvest_time_entry_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestTimeEntry *self = HARVEST_TIME_ENTRY(obj);

	switch (prop_id) {
	case PROP_ID:
		g_value_set_int(val, self->id);
		break;
	case PROP_SPENT_DATE:
		g_value_set_boxed(val, self->spent_date);
		break;
	case PROP_USER:
		g_value_set_object(val, self->user);
		break;
	case PROP_USER_ASSIGNMENT:
		g_value_set_object(val, self->user_assignment);
		break;
	case PROP_CLIENT:
		g_value_set_object(val, self->client);
		break;
	case PROP_PROJECT:
		g_value_set_object(val, self->project);
		break;
	case PROP_TASK:
		g_value_set_object(val, self->task);
		break;
	case PROP_TASK_ASSIGNMEMT:
		g_value_set_object(val, self->task_assingment);
		break;
	case PROP_INVOICE:
		g_value_set_object(val, self->invoice);
		break;
	case PROP_HOURS:
		g_value_set_double(val, self->hours);
		break;
	case PROP_NOTES:
		g_value_set_string(val, self->notes);
		break;
	case PROP_IS_LOCKED:
		g_value_set_boolean(val, self->is_locked);
		break;
	case PROP_LOCKED_REASON:
		g_value_set_string(val, self->locked_reason);
		break;
	case PROP_IS_CLOSED:
		g_value_set_boolean(val, self->is_closed);
		break;
	case PROP_IS_BILLED:
		g_value_set_boolean(val, self->is_billed);
		break;
	case PROP_TIMER_STARTED_AT:
		g_value_set_boxed(val, self->timer_started_at);
		break;
	case PROP_STARTED_TIME:
		g_value_set_boxed(val, self->started_time);
		break;
	case PROP_ENDED_TIME:
		g_value_set_boxed(val, self->ended_time);
		break;
	case PROP_IS_RUNNING:
		g_value_set_boolean(val, self->is_running);
		break;
	case PROP_BILLABLE:
		g_value_set_boolean(val, self->billable);
		break;
	case PROP_BUDGETED:
		g_value_set_boolean(val, self->budgeted);
		break;
	case PROP_BILLABLE_RATE:
		g_value_set_double(val, self->billable_rate);
		break;
	case PROP_COST_RATE:
		g_value_set_double(val, self->cost_rate);
		break;
	case PROP_CREATED_AT:
		g_value_set_boxed(val, self->created_at);
		break;
	case PROP_UPDATED_AT:
		g_value_set_boxed(val, self->updated_at);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_time_entry_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestTimeEntry *self = HARVEST_TIME_ENTRY(obj);

	switch (prop_id) {
	case PROP_ID:
		self->id = g_value_get_int(val);
		break;
	case PROP_SPENT_DATE:
		if (self->spent_date != NULL)
			g_date_time_unref(self->spent_date);
		self->spent_date = g_value_dup_boxed(val);
		break;
	case PROP_USER:
		if (self->user != NULL)
			g_object_unref(self->user);
		self->user = g_value_get_object(val);
		break;
	case PROP_USER_ASSIGNMENT:
		if (self->user_assignment != NULL)
			g_object_unref(self->user_assignment);
		self->user_assignment = g_value_get_object(val);
		break;
	case PROP_CLIENT:
		if (self->client != NULL)
			g_object_unref(self->client);
		self->client = g_value_get_object(val);
		break;
	case PROP_PROJECT:
		if (self->project != NULL)
			g_object_unref(self->project);
		self->project = g_value_get_object(val);
		break;
	case PROP_TASK:
		if (self->task != NULL)
			g_object_unref(self->task);
		self->task = g_value_get_object(val);
		break;
	case PROP_TASK_ASSIGNMEMT:
		if (self->task_assingment != NULL)
			g_object_unref(self->task_assingment);
		self->task_assingment = g_value_get_object(val);
		break;
	case PROP_INVOICE:
		if (self->invoice != NULL)
			g_object_unref(self->invoice);
		self->invoice = g_value_get_object(val);
		break;
	case PROP_HOURS:
		self->hours = g_value_get_double(val);
		break;
	case PROP_NOTES:
		g_free(self->notes);
		self->notes = g_value_dup_string(val);
		break;
	case PROP_IS_LOCKED:
		self->is_locked = g_value_get_boolean(val);
		break;
	case PROP_LOCKED_REASON:
		g_free(self->locked_reason);
		self->locked_reason = g_value_dup_string(val);
		break;
	case PROP_IS_CLOSED:
		self->is_closed = g_value_get_boolean(val);
		break;
	case PROP_IS_BILLED:
		self->is_billed = g_value_get_boolean(val);
		break;
	case PROP_TIMER_STARTED_AT:
		if (self->timer_started_at != NULL)
			g_date_time_unref(self->timer_started_at);
		self->timer_started_at = g_value_dup_boxed(val);
		break;
	case PROP_STARTED_TIME:
		if (self->started_time != NULL)
			g_date_time_unref(self->started_time);
		self->started_time = g_value_dup_boxed(val);
		break;
	case PROP_ENDED_TIME:
		if (self->ended_time != NULL)
			g_date_time_unref(self->ended_time);
		self->ended_time = g_value_dup_boxed(val);
		break;
	case PROP_IS_RUNNING:
		self->is_running = g_value_get_boolean(val);
		break;
	case PROP_BILLABLE:
		self->billable = g_value_get_boolean(val);
		break;
	case PROP_BUDGETED:
		self->budgeted = g_value_get_boolean(val);
		break;
	case PROP_BILLABLE_RATE:
		self->billable_rate = g_value_get_double(val);
		break;
	case PROP_COST_RATE:
		self->cost_rate = g_value_get_double(val);
		break;
	case PROP_CREATED_AT:
		if (self->created_at != NULL)
			g_date_time_unref(self->created_at);
		self->created_at = g_value_dup_boxed(val);
		break;
	case PROP_UPDATED_AT:
		if (self->updated_at != NULL)
			g_date_time_unref(self->updated_at);
		self->updated_at = g_value_dup_boxed(val);
		break;
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

	obj_properties[PROP_ID] = g_param_spec_int("id", _("ID"), _("Unique ID for the time entry."), 0,
		INT_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_SPENT_DATE]
		= g_param_spec_boxed("spent_date", _("Spent Date"), _("Date of the time entry."),
			G_TYPE_DATE_TIME, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_USER] = g_param_spec_object("user", _("User"),
		_("An object containing the id and name of the associated user."), HARVEST_TYPE_USER,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_USER_ASSIGNMENT]
		= g_param_spec_object("user_assignment", _("User Assignment"),
			_("A user assignment object of the associated user."), HARVEST_TYPE_USER_ASSIGNMENT,
			G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_CLIENT]	 = g_param_spec_object("client", _("Client"),
		 _("An object containing the id and name of the associated client."), HARVEST_TYPE_CLIENT,
		 G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_PROJECT] = g_param_spec_object("project", _("Project"),
		_("An object containing the id and name of the associated project."), HARVEST_TYPE_PROJECT,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_TASK]	 = g_param_spec_object("task", _("Task"),
		   _("An object containing the id and name of the associated task."), HARVEST_TYPE_TASK,
		   G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_TASK_ASSIGNMEMT]
		= g_param_spec_object("task_assignment", _("Task Assignment"),
			_("A task assignment object of the associated task."), HARVEST_TYPE_TASK_ASSIGNMENT,
			G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_INVOICE] = g_param_spec_object("invoice", _("Invoice"),
		_("Once the time entry has been invoiced, this field will include the associated invoice’s "
		  "id and number."),
		HARVEST_TYPE_INVOICE, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_HOURS]	 = g_param_spec_double("hours", _("Hours"),
		  _("Number of (decimal time) hours tracked in this time entry."), 0, DBL_MAX, 0,
		  G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_NOTES]
		= g_param_spec_string("notes", _("Notes"), _("Notes attached to the time entry."), NULL,
			G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_IS_LOCKED]	   = g_param_spec_boolean("is_locked", _("Is Locked"),
		_("Whether or not the time entry has been locked."), FALSE,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_LOCKED_REASON] = g_param_spec_string("locked_reason", _("Locked Reason"),
		_("Why the time entry has been locked."), NULL,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_IS_CLOSED]	   = g_param_spec_boolean("is_closed", _("Is Closed"),
		_("Whether or not the time entry has been approved via Timesheet Approval."), FALSE,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_IS_BILLED]	   = g_param_spec_boolean("is_billed", _("Is Billed"),
		_("Whether or not the time entry has been marked as invoiced."), FALSE,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_TIMER_STARTED_AT]
		= g_param_spec_boxed("timer_started_at", _("Timer Started At"),
			_("Date and time the timer was started (if tracking by duration). Use the ISO 8601 "
			  "Format."),
			G_TYPE_DATE_TIME, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_STARTED_TIME]  = g_param_spec_boxed("started_time", _("Started Time"),
		 _("Time the time entry was started (if tracking by start/end times)."), G_TYPE_DATE_TIME,
		 G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_ENDED_TIME]	   = g_param_spec_boxed("ended_time", _("Ended Time"),
		   _("Time the time entry was ended (if tracking by start/end times)."), G_TYPE_DATE_TIME,
		   G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_IS_RUNNING]	   = g_param_spec_boolean("is_running", _("Is Running"),
		   _("Whether or not the time entry is currently running."), FALSE,
		   G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_BILLABLE]	   = g_param_spec_boolean("billable", _("Billable"),
		 _("Whether or not the time entry is billable."), FALSE,
		 G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_BUDGETED]	   = g_param_spec_boolean("budgeted", _("Budgeted"),
		 _("Whether or not the time entry counts towards the project budget."), FALSE,
		 G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_BILLABLE_RATE] = g_param_spec_double("billable_rate", _("Billable Rate"),
		_("The billable rate for the time entry."), 0, DBL_MAX, 0,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_COST_RATE]
		= g_param_spec_double("cost_rate", _("Cost Rate"), _("The cost rate for the time entry."),
			0, DBL_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_CREATED_AT] = g_param_spec_boxed("created_at", _("Created At"),
		_("Date and time the time entry was created. Use the ISO 8601 Format."), G_TYPE_DATE_TIME,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_UPDATED_AT] = g_param_spec_boxed("updated_at", _("Updated At"),
		_("Date and time the time entry was last updated. Use the ISO 8601 Format."),
		G_TYPE_DATE_TIME, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_time_entry_init(G_GNUC_UNUSED HarvestTimeEntry *self)
{}
