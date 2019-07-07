#include <float.h>
#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-task.h"

struct _HarvestTask
{
	GObject parent_instance;

	int id;
	char *name;
	gboolean billable_by_default;
	double default_hourly_rate;
	gboolean is_default;
	gboolean is_active;
	GDateTime *created_at;
	GDateTime *updated_at;
};

G_DEFINE_TYPE(HarvestTask, harvest_task, G_TYPE_OBJECT)

enum HarvestTaskProps
{
	PROP_0,
	PROP_ID,
	PROP_NAME,
	PROP_BILLABLE_BY_DEFAULT,
	PROP_DEFAULT_HOURLY_RATE,
	PROP_IS_DEFAULT,
	PROP_IS_ACTIVE,
	PROP_CREATED_AT,
	PROP_UPDATED_AT,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_task_finalize(GObject *obj)
{
	HarvestTask *self = HARVEST_TASK(obj);

	g_free(self->name);
	if (self->created_at != NULL)
		g_date_time_unref(self->created_at);
	if (self->updated_at != NULL)
		g_date_time_unref(self->updated_at);
}

static void
harvest_task_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestTask *self = HARVEST_TASK(obj);

	switch (prop_id) {
	case PROP_ID:
		g_value_set_int(val, self->id);
		break;
	case PROP_NAME:
		g_value_set_string(val, self->name);
		break;
	case PROP_BILLABLE_BY_DEFAULT:
		g_value_set_boolean(val, self->billable_by_default);
		break;
	case PROP_DEFAULT_HOURLY_RATE:
		g_value_set_double(val, self->default_hourly_rate);
		break;
	case PROP_IS_DEFAULT:
		g_value_set_boolean(val, self->is_default);
		break;
	case PROP_IS_ACTIVE:
		g_value_set_boolean(val, self->is_active);
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
harvest_task_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestTask *self = HARVEST_TASK(obj);

	switch (prop_id) {
	case PROP_ID:
		self->id = g_value_get_int(val);
		break;
	case PROP_NAME:
		g_free(self->name);
		self->name = g_value_dup_string(val);
		break;
	case PROP_BILLABLE_BY_DEFAULT:
		self->billable_by_default = g_value_get_boolean(val);
		break;
	case PROP_DEFAULT_HOURLY_RATE:
		self->default_hourly_rate = g_value_get_double(val);
		break;
	case PROP_IS_DEFAULT:
		self->is_default = g_value_get_boolean(val);
		break;
	case PROP_IS_ACTIVE:
		self->is_active = g_value_get_boolean(val);
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
harvest_task_class_init(HarvestTaskClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_task_finalize;
	obj_class->get_property = harvest_task_get_property;
	obj_class->set_property = harvest_task_set_property;

	obj_properties[PROP_ID]   = g_param_spec_int("id", _("ID"), _("Unique ID for the task."), 0,
		  INT_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_NAME] = g_param_spec_string(
		"name", _("Name"), _("The name of the task."), NULL, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_BILLABLE_BY_DEFAULT] =
		g_param_spec_boolean("billable_by_default", _("Billable by Default"),
			_("Used in determining whether default tasks should be marked billable when creating a "
			  "new project."),
			FALSE, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_DEFAULT_HOURLY_RATE] =
		g_param_spec_double("default_hourly_rate", _("Default Hourly Rate"),
			_("The hourly rate to use for this task when it is added to a project."), 0, DBL_MAX, 0,
			G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_IS_DEFAULT] = g_param_spec_boolean("is_default", _("Is Default"),
		_("Whether this task should be automatically added to future projects."), FALSE,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_IS_ACTIVE]  = g_param_spec_boolean("is_active", _("Is Active"),
		 _("Whether this task is active or archived."), FALSE,
		 G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_CREATED_AT] =
		g_param_spec_boxed("created_at", _("Created At"), _("Date and time the task was created."),
			G_TYPE_DATE_TIME, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_UPDATED_AT] = g_param_spec_boxed("updated_at", _("Created At"),
		_("Date and time the task was last updated."), G_TYPE_DATE_TIME,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_task_init(G_GNUC_UNUSED HarvestTask *self)
{}
