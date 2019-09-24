#include "config.h"

#define G_LOG_DOMAIN "HarvestProject"

#include <float.h>
#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n-lib.h>
#include <json-glib/json-glib.h>

#include "harvest-client.h"
#include "harvest-common.h"
#include "harvest-project.h"

struct _HarvestProject
{
	GObject parent_instance;

	int id;
	HarvestClient *client;
	char *name;
	char *code;
	gboolean is_active : 1;
	gboolean is_billable : 1;
	gboolean is_fixed_fee : 1;
	char *bill_by;
	double hourly_rate;
	double budget;
	char *budget_by;
	gboolean budget_is_monthly : 1;
	gboolean notify_when_over_budget : 1;
	double over_budget_notification_percentage;
	GDateTime *over_budget_notification_date;
	gboolean show_budget_to_all : 1;
	double cost_budget;
	gboolean cost_budget_include_expenses : 1;
	double fee;
	char *notes;
	GDateTime *starts_on;
	GDateTime *ends_on;
	GDateTime *created_at;
	GDateTime *updated_at;
};

static void harvest_project_json_serializable_init(JsonSerializableIface *iface);

G_DEFINE_TYPE_WITH_CODE(HarvestProject, harvest_project, G_TYPE_OBJECT,
	G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, harvest_project_json_serializable_init))

enum HarvestProjectProps
{
	PROP_0,
	PROP_ID,
	PROP_CLIENT,
	PROP_NAME,
	PROP_CODE,
	PROP_IS_ACTIVE,
	PROP_IS_BILLABLE,
	PROP_IS_FIXED_FEE,
	PROP_BILL_BY,
	PROP_HOURLY_RATE,
	PROP_BUDGET,
	PROP_BUDGET_BY,
	PROP_BUDGET_IS_MONTHLY,
	PROP_NOTIFY_WHEN_OVER_BUDGET,
	PROP_OVER_BUDGET_NOTIFICATION_PERCENTAGE,
	PROP_OVER_BUDGET_NOTIFICATION_DATE,
	PROP_SHOW_BUDGET_TO_ALL,
	PROP_COST_BUDGET,
	PROP_COST_BUDGET_INCLUDE_EXPENSES,
	PROP_FEE,
	PROP_NOTES,
	PROP_STARTS_ON,
	PROP_ENDS_ON,
	PROP_CREATED_AT,
	PROP_UPDATED_AT,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static gboolean
harvest_project_deserialize_property(JsonSerializable *serializable, const gchar *prop_name,
	GValue *val, GParamSpec *pspec, JsonNode *prop_node)
{
	if (g_strcmp0(prop_name, "created_at") == 0 || g_strcmp0(prop_name, "updated_at") == 0) {
		const GDateTime *dt = g_date_time_new_from_iso8601(json_node_get_string(prop_node), NULL);
		g_value_set_boxed(val, dt);

		return TRUE;
	} else if (g_strcmp0(prop_name, "client") == 0) {
		GObject *obj = json_gobject_deserialize(HARVEST_TYPE_CLIENT, prop_node);
		g_value_set_object(val, obj);

		return TRUE;
	} else if (g_strcmp0(prop_name, "over_budget_notification_date") == 0
			   || g_strcmp0(prop_name, "starts_on") == 0 || g_strcmp0(prop_name, "ends_on") == 0) {
		const GDateTime *dt
			= g_date_time_new_from_abbreviated_date(json_node_get_string(prop_node));
		g_value_set_boxed(val, dt);

		return TRUE;
	}

	return json_serializable_default_deserialize_property(
		serializable, prop_name, val, pspec, prop_node);
}

static void
harvest_project_json_serializable_init(JsonSerializableIface *iface)
{
	iface->deserialize_property = harvest_project_deserialize_property;
}

static void
harvest_project_finalize(GObject *obj)
{
	HarvestProject *self = HARVEST_PROJECT(obj);

	if (self->client != NULL)
		g_object_unref(self->client);
	g_free(self->name);
	g_free(self->code);
	g_free(self->bill_by);
	g_free(self->budget_by);
	if (self->over_budget_notification_date != NULL)
		g_date_time_unref(self->over_budget_notification_date);
	g_free(self->notes);
	if (self->starts_on != NULL)
		g_date_time_unref(self->starts_on);
	if (self->ends_on != NULL)
		g_date_time_unref(self->ends_on);
	if (self->created_at != NULL)
		g_date_time_unref(self->created_at);
	if (self->updated_at != NULL)
		g_date_time_unref(self->updated_at);

	G_OBJECT_CLASS(harvest_project_parent_class)->finalize(obj);
}

static void
harvest_project_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestProject *self = HARVEST_PROJECT(obj);

	switch (prop_id) {
	case PROP_ID:
		g_value_set_int(val, self->id);
		break;
	case PROP_CLIENT:
		g_value_set_object(val, self->client);
		break;
	case PROP_NAME:
		g_value_set_string(val, self->name);
		break;
	case PROP_CODE:
		g_value_set_string(val, self->code);
		break;
	case PROP_IS_ACTIVE:
		g_value_set_boolean(val, self->is_active);
		break;
	case PROP_IS_BILLABLE:
		g_value_set_boolean(val, self->is_billable);
		break;
	case PROP_IS_FIXED_FEE:
		g_value_set_boolean(val, self->is_fixed_fee);
		break;
	case PROP_BILL_BY:
		g_value_set_string(val, self->bill_by);
		break;
	case PROP_HOURLY_RATE:
		g_value_set_double(val, self->hourly_rate);
		break;
	case PROP_BUDGET:
		g_value_set_double(val, self->budget);
		break;
	case PROP_BUDGET_BY:
		g_value_set_string(val, self->budget_by);
		break;
	case PROP_BUDGET_IS_MONTHLY:
		g_value_set_boolean(val, self->budget_is_monthly);
		break;
	case PROP_NOTIFY_WHEN_OVER_BUDGET:
		g_value_set_boolean(val, self->notify_when_over_budget);
		break;
	case PROP_OVER_BUDGET_NOTIFICATION_PERCENTAGE:
		g_value_set_double(val, self->over_budget_notification_percentage);
		break;
	case PROP_OVER_BUDGET_NOTIFICATION_DATE:
		g_value_set_boxed(val, self->over_budget_notification_date);
		break;
	case PROP_SHOW_BUDGET_TO_ALL:
		g_value_set_boolean(val, self->show_budget_to_all);
		break;
	case PROP_COST_BUDGET:
		g_value_set_double(val, self->cost_budget);
		break;
	case PROP_COST_BUDGET_INCLUDE_EXPENSES:
		g_value_set_boolean(val, self->cost_budget_include_expenses);
		break;
	case PROP_FEE:
		g_value_set_double(val, self->fee);
		break;
	case PROP_NOTES:
		g_value_set_string(val, self->notes);
		break;
	case PROP_STARTS_ON:
		g_value_set_boxed(val, self->starts_on);
		break;
	case PROP_ENDS_ON:
		g_value_set_boxed(val, self->ends_on);
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
harvest_project_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestProject *self = HARVEST_PROJECT(obj);

	switch (prop_id) {
	case PROP_ID:
		self->id = g_value_get_int(val);
		break;
	case PROP_CLIENT:
		if (self->client != NULL)
			g_object_unref(self->client);
		self->client = g_value_dup_object(val);
		break;
	case PROP_NAME:
		g_free(self->name);
		self->name = g_value_dup_string(val);
		break;
	case PROP_CODE:
		g_free(self->code);
		self->code = g_value_dup_string(val);
		break;
	case PROP_IS_ACTIVE:
		self->is_active = g_value_get_boolean(val);
		break;
	case PROP_IS_BILLABLE:
		self->is_billable = g_value_get_boolean(val);
		break;
	case PROP_IS_FIXED_FEE:
		self->is_fixed_fee = g_value_get_boolean(val);
		break;
	case PROP_BILL_BY:
		g_free(self->bill_by);
		self->bill_by = g_value_dup_string(val);
		break;
	case PROP_HOURLY_RATE:
		self->hourly_rate = g_value_get_double(val);
		break;
	case PROP_BUDGET:
		self->budget = g_value_get_boolean(val);
		break;
	case PROP_BUDGET_BY:
		g_free(self->budget_by);
		self->budget_by = g_value_dup_string(val);
		break;
	case PROP_BUDGET_IS_MONTHLY:
		self->budget_is_monthly = g_value_get_boolean(val);
		break;
	case PROP_NOTIFY_WHEN_OVER_BUDGET:
		self->notify_when_over_budget = g_value_get_boolean(val);
		break;
	case PROP_OVER_BUDGET_NOTIFICATION_PERCENTAGE:
		self->over_budget_notification_percentage = g_value_get_double(val);
		break;
	case PROP_OVER_BUDGET_NOTIFICATION_DATE:
		if (self->over_budget_notification_date != NULL)
			g_date_time_unref(self->over_budget_notification_date);
		self->over_budget_notification_date = g_value_get_boxed(val);
		break;
	case PROP_SHOW_BUDGET_TO_ALL:
		self->show_budget_to_all = g_value_get_boolean(val);
		break;
	case PROP_COST_BUDGET:
		self->cost_budget = g_value_get_double(val);
		break;
	case PROP_COST_BUDGET_INCLUDE_EXPENSES:
		self->cost_budget_include_expenses = g_value_get_boolean(val);
		break;
	case PROP_FEE:
		self->fee = g_value_get_double(val);
		break;
	case PROP_NOTES:
		g_free(self->notes);
		self->notes = g_value_dup_string(val);
		break;
	case PROP_STARTS_ON:
		if (self->starts_on != NULL)
			g_date_time_unref(self->starts_on);
		self->starts_on = g_value_get_boxed(val);
		break;
	case PROP_ENDS_ON:
		if (self->ends_on != NULL)
			g_date_time_unref(self->ends_on);
		self->ends_on = g_value_get_boxed(val);
		break;
	case PROP_CREATED_AT:
		if (self->created_at != NULL)
			g_date_time_unref(self->created_at);
		self->created_at = g_value_get_boxed(val);
		break;
	case PROP_UPDATED_AT:
		if (self->updated_at != NULL)
			g_date_time_unref(self->updated_at);
		self->updated_at = g_value_get_boxed(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_project_class_init(HarvestProjectClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_project_finalize;
	obj_class->get_property = harvest_project_get_property;
	obj_class->set_property = harvest_project_set_property;

	obj_properties[PROP_ID] = g_param_spec_int("id", _("ID"), _("Unique ID for the project."), 0,
		INT_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_CLIENT] = g_param_spec_object("client", _("Client"),
		_("An object containing the project’s client id, name, and currency."), HARVEST_TYPE_CLIENT,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_NAME]
		= g_param_spec_string("name", _("Name"), _("Unique name for the project."), NULL,
			G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_CODE]
		= g_param_spec_string("code", _("Code"), _("The code associated with the project."), NULL,
			G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_IS_ACTIVE]		   = g_param_spec_boolean("is_active", _("Is Active"),
		_("Whether the project is active or archived."), FALSE,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_IS_BILLABLE]	   = g_param_spec_boolean("is_billable", _("Is Billable"),
		  _("Whether the project is billable or not."), FALSE,
		  G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_IS_FIXED_FEE]	  = g_param_spec_boolean("is_fixed_fee", _("Is Fixed Fee"),
		 _("Whether the project is a fixed-fee project or not."), FALSE,
		 G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_BILL_BY]		   = g_param_spec_string("bill_by", _("Bill By"),
		  _("The method by which the project is invoiced."), NULL,
		  G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_HOURLY_RATE]	   = g_param_spec_double("hourly_rate", _("Hourly Rate"),
		  _("Rate for projects billed by Project Hourly Rate."), 0, DBL_MAX, 0,
		  G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_BUDGET]			   = g_param_spec_double("budget", _("Budget"),
		   _("The budget in hours for the project when budgeting by time."), 0, DBL_MAX, 0,
		   G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_BUDGET_BY]		   = g_param_spec_string("budget_by", _("Budget By"),
		_("The method by which the project is budgeted."), FALSE,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_BUDGET_IS_MONTHLY] = g_param_spec_boolean("budget_is_monthly",
		_("Budget is Monthly"), _("Option to have the budget reset every month."), FALSE,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_NOTIFY_WHEN_OVER_BUDGET]
		= g_param_spec_boolean("notify_when_over_budget", _("Notify When Over Budget"),
			_("Whether project managers should be notified when the project goes over budget."),
			FALSE, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_OVER_BUDGET_NOTIFICATION_PERCENTAGE] = g_param_spec_double(
		"over_budget_notification_percentage", _("Over Budget Notification Percentage"),
		_("Percentage value used to trigger over budget email alerts."), 0, DBL_MAX, 0,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_OVER_BUDGET_NOTIFICATION_DATE]
		= g_param_spec_boxed("over_budget_notification_date", _("Over Budget Notification Date"),
			_("Date of last over budget notification. If none have been sent, this will be null."),
			G_TYPE_DATE_TIME, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_SHOW_BUDGET_TO_ALL]
		= g_param_spec_boolean("show_budget_to_all", _("Show Budget to All"),
			_("Option to show project budget to all employees. Does not apply to Total Project Fee "
			  "projects."),
			FALSE, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_COST_BUDGET] = g_param_spec_double("cost_budget", _("Cost Budget"),
		_("The monetary budget for the project when budgeting by money."), 0, DBL_MAX, 0,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_COST_BUDGET_INCLUDE_EXPENSES]
		= g_param_spec_boolean("cost_budget_includes_expenses", _("Cost Budget Includes Expenses"),
			_("Option for budget of Total Project Fees projects to include tracked expenses."),
			FALSE, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_FEE]   = g_param_spec_double("fee", _("Fee"),
		  _("The amount you plan to invoice for the project. Only used by fixed-fee projects."), 0,
		  DBL_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_NOTES] = g_param_spec_string("notes", _("Notes"), _("Project notes."), NULL,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_STARTS_ON]
		= g_param_spec_boxed("starts_on", _("Starts On"), _("Date the project was started."),
			G_TYPE_DATE_TIME, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_ENDS_ON]
		= g_param_spec_boxed("ends_on", _("Ends On"), _("Date the project will end."),
			G_TYPE_DATE_TIME, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_CREATED_AT] = g_param_spec_boxed("created_at", _("Created At"),
		_("Date and time the project was created."), G_TYPE_DATE_TIME,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_UPDATED_AT] = g_param_spec_boxed("updated_at", _("Updated At"),
		_("Date and time the project was last updated."), G_TYPE_DATE_TIME,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_project_init(G_GNUC_UNUSED HarvestProject *self)
{}
