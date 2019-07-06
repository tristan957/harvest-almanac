#include <float.h>
#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-project.h"
#include "harvest-task-assignment.h"
#include "harvest-task.h"

struct _HarvestTaskAssignment
{
	int id;
	HarvestProject *project;
	HarvestTask *task;
	gboolean is_active;
	gboolean billable;
	double hourly_rate;
	double budget;
	GDateTime *created_at;
	GDateTime *updated_at;
};

G_DEFINE_TYPE(HarvestTaskAssignment, harvest_task_assignment, G_TYPE_OBJECT)

enum HarvestTaskAssignmentProps
{
	PROP_0,
	PROP_ID,
	PROP_PROJECT,
	PROP_TASK,
	PROP_IS_ACTIVE,
	PROP_BILLABLE,
	PROP_HOURLY_RATE,
	PROP_BUDGET,
	PROP_CREATED_AT,
	PROP_UPDATED_AT,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_task_assignment_finalize(GObject *obj)
{
	HarvestTaskAssignment *self = HARVEST_TASK_ASSIGNMENT(obj);

	if (self->project != NULL)
		g_object_unref(self->project);
	if (self->task != NULL)
		g_object_unref(self->task);
	if (self->created_at != NULL)
		g_date_time_unref(self->created_at);
	if (self->updated_at != NULL)
		g_date_time_unref(self->updated_at);

	G_OBJECT_CLASS(harvest_task_assignment_parent_class)->finalize(obj);
}

static void
harvest_task_assignment_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestTaskAssignment *self = HARVEST_TASK_ASSIGNMENT(obj);

	switch (prop_id) {
	case PROP_ID:
		g_value_set_int(val, self->id);
		break;
	case PROP_PROJECT:
		g_value_set_object(val, self->project);
		break;
	case PROP_TASK:
		g_value_set_object(val, self->task);
		break;
	case PROP_IS_ACTIVE:
		g_value_set_boolean(val, self->is_active);
		break;
	case PROP_BILLABLE:
		g_value_set_boolean(val, self->billable);
		break;
	case PROP_HOURLY_RATE:
		g_value_set_double(val, self->hourly_rate);
		break;
	case PROP_BUDGET:
		g_value_set_double(val, self->budget);
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
harvest_task_assignment_set_property(
	GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestTaskAssignment *self = HARVEST_TASK_ASSIGNMENT(obj);

	switch (prop_id) {
	case PROP_ID:
		self->id = g_value_get_int(val);
		break;
	case PROP_PROJECT:
		if (self->project != NULL)
			g_object_unref(self->project);
		self->project = g_value_dup_object(val);
		break;
	case PROP_TASK:
		if (self->task != NULL)
			g_object_unref(self->task);
		self->task = g_value_dup_object(val);
		break;
	case PROP_IS_ACTIVE:
		self->is_active = g_value_get_boolean(val);
		break;
	case PROP_BILLABLE:
		self->billable = g_value_get_boolean(val);
		break;
	case PROP_HOURLY_RATE:
		self->hourly_rate = g_value_get_double(val);
		break;
	case PROP_BUDGET:
		self->budget = g_value_get_double(val);
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
harvest_task_assignment_class_init(HarvestTaskAssignmentClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_task_assignment_finalize;
	obj_class->get_property = harvest_task_assignment_get_property;
	obj_class->set_property = harvest_task_assignment_set_property;

	obj_properties[PROP_ID] =
		g_param_spec_int("id", _("ID"), _("Unique ID for the task assignment."), 0, INT_MAX, 0,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_PROJECT]	 = g_param_spec_object("project", _("Project"),
		_("An object containing the id, name, and code of the associated project."),
		HARVEST_TYPE_PROJECT, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_TASK]		 = g_param_spec_object("task", _("Task"),
		   _("An object containing the id and name of the associated task."), HARVEST_TYPE_TASK,
		   G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_IS_ACTIVE]   = g_param_spec_boolean("is_active", _("Is Active"),
		  _("Whether the task assignment is active or archived."), FALSE,
		  G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_BILLABLE]	= g_param_spec_boolean("billable", _("Billable"),
		   _("Whether the task assignment is billable or not. For example: if set to true, all time "
			 "tracked on this project for the associated task will be marked as billable."),
		   FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_HOURLY_RATE] = g_param_spec_double("hourly_rate", _("Hourly Rate"),
		_("Rate used when the project’s bill_by is Tasks."), 0, DBL_MAX, 0,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_BUDGET]		 = g_param_spec_double("budget", _("Budget"),
		 _("Budget used when the project’s budget_by is task or task_fees."), 0, DBL_MAX, 0,
		 G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_CREATED_AT]  = g_param_spec_boxed("created_at", _("Created At"),
		 _("Date and time the task assignment was created."), G_TYPE_DATE_TIME,
		 G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_UPDATED_AT]  = g_param_spec_boxed("updated_at", _("Updated At"),
		 _("Date and time the task assignment was last updated."), G_TYPE_DATE_TIME,
		 G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_task_assignment_init(G_GNUC_UNUSED HarvestTaskAssignment *self)
{}
