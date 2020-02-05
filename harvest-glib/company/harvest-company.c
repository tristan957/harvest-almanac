#include "harvest-glib/config.h"

#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>
#include <json-glib/json-glib.h>

#include "harvest-glib/company/harvest-company-enum-types.h"
#include "harvest-glib/company/harvest-company.h"
#include "harvest-glib/company/requests/harvest-get-company-request.h"

struct _HarvestCompany
{
	GObject parent_instance;

	char *base_uri;
	char *full_domain;
	char *name;
	gboolean is_active : 1;
	HarvestWeekStartDay week_start_day;
	gboolean wants_timestamp_timers : 1;
	HarvestTimeFormat time_format;
	HarvestPlanType plan_type;
	HarvestClock clock;
	char *decimal_symbol;
	char *thousands_operator;
	char *color_scheme;
	unsigned int weekly_capacity;
	gboolean exepnse_feature : 1;
	gboolean invoice_feature : 1;
	gboolean estimate_feature : 1;
	gboolean approval_feature : 1;
};

static void harvest_company_json_serializable_init(JsonSerializableIface *iface);

G_DEFINE_TYPE_WITH_CODE(HarvestCompany, harvest_company, G_TYPE_OBJECT,
	G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, harvest_company_json_serializable_init))

enum HarvestCompanyProps
{
	PROP_0,
	PROP_BASE_URI,
	PROP_FULL_DOMAIN,
	PROP_NAME,
	PROP_IS_ACTIVE,
	PROP_WEEK_START_DAY,
	PROP_WANTS_TIMESTAMP_TIMERS,
	PROP_TIME_FORMAT,
	PROP_PLAN_TYPE,
	PROP_CLOCK,
	PROP_DECIMAL_SYMBOL,
	PROP_THOUSANDS_OPERATOR,
	PROP_COLOR_SCHEME,
	PROP_WEEKLY_CAPACITY,
	PROP_EXPENSE_FEATURE,
	PROP_INVOICE_FEATURE,
	PROP_ESTIMATE_FEATURE,
	PROP_APPROVAL_FEATURE,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static gboolean
harvest_company_json_deserialize_property(JsonSerializable *serializable, const gchar *prop_name,
	GValue *val, GParamSpec *pspec, JsonNode *prop_node)
{
	if (g_strcmp0(prop_name, "week-start-day") == 0) {
		const char *week_start_day = json_node_get_string(prop_node);
		if (g_strcmp0(week_start_day, "Saturday") == 0) {
			g_value_set_enum(val, (gint) HARVEST_WEEK_START_DAY_SATURDAY);
		} else if (g_strcmp0(week_start_day, "Sunday") == 0) {
			g_value_set_enum(val, (gint) HARVEST_WEEK_START_DAY_SUNDAY);
		} else if (g_strcmp0(week_start_day, "Monday") == 0) {
			g_value_set_enum(val, (gint) HARVEST_WEEK_START_DAY_MONDAY);
		} else {
			return FALSE;
		}

		return TRUE;
	} else if (g_strcmp0(prop_name, "time-format") == 0) {
		const char *time_format = json_node_get_string(prop_node);
		if (g_strcmp0(time_format, "decimal") == 0) {
			g_value_set_enum(val, (gint) HARVEST_TIME_FORMAT_DECIMAL);
		} else if (g_strcmp0(time_format, "hours_minutes") == 0) {
			g_value_set_enum(val, (gint) HARVEST_TIME_FORMAT_HOURS_MINUTES);
		} else {
			return FALSE;
		}

		return TRUE;
	} else if (g_strcmp0(prop_name, "plan-type") == 0) {
		const char *plan_type = json_node_get_string(prop_node);
		if (g_strcmp0(plan_type, "trial") == 0) {
			g_value_set_enum(val, (gint) HARVEST_PLAN_TYPE_TRIAL);
		} else if (g_strcmp0(plan_type, "free") == 0) {
			g_value_set_enum(val, (gint) HARVEST_PLAN_TYPE_FREE);
		} else if (g_strcmp0(plan_type, "business") == 0) {
			g_value_set_enum(val, (gint) HARVEST_PLAN_TYPE_BUSINESS);
		} else if (g_strcmp0(plan_type, "simple-v4") == 0) {
			g_value_set_enum(val, (gint) HARVEST_PLAN_TYPE_SIMPLE_V4);
		} else {
			return FALSE;
		}

		return TRUE;
	} else if (g_strcmp0(prop_name, "clock") == 0) {
		const char *clock = json_node_get_string(prop_node);
		if (g_strcmp0(clock, "12h") == 0) {
			g_value_set_enum(val, (gint) HARVEST_CLOCK_12H);
		} else if (g_strcmp0(clock, "24h") == 0) {
			g_value_set_enum(val, (gint) HARVEST_CLOCK_24H);
		} else {
			return FALSE;
		}

		return TRUE;
	}

	return json_serializable_default_deserialize_property(
		serializable, prop_name, val, pspec, prop_node);
}

static JsonNode *
harvest_company_json_serialize_property(
	JsonSerializable *serializable, const gchar *prop_name, const GValue *val, GParamSpec *pspec)
{
	return json_serializable_default_serialize_property(serializable, prop_name, val, pspec);
}

static void
harvest_company_json_serializable_init(JsonSerializableIface *iface)
{
	iface->deserialize_property = harvest_company_json_deserialize_property;
	iface->serialize_property	= harvest_company_json_serialize_property;
}

static void
harvest_company_finalize(GObject *obj)
{
	HarvestCompany *self = HARVEST_COMPANY(obj);

	g_free(self->base_uri);
	g_free(self->full_domain);
	g_free(self->name);
	g_free(self->decimal_symbol);
	g_free(self->thousands_operator);
	g_free(self->color_scheme);

	G_OBJECT_CLASS(harvest_company_parent_class)->finalize(obj);
}

static void
harvest_company_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestCompany *self = HARVEST_COMPANY(obj);

	switch (prop_id) {
	case PROP_BASE_URI:
		g_value_set_string(val, self->base_uri);
		break;
	case PROP_FULL_DOMAIN:
		g_value_set_string(val, self->full_domain);
		break;
	case PROP_NAME:
		g_value_set_string(val, self->name);
		break;
	case PROP_IS_ACTIVE:
		g_value_set_boolean(val, self->is_active);
		break;
	case PROP_WEEK_START_DAY:
		g_value_set_enum(val, (int) self->week_start_day);
		break;
	case PROP_WANTS_TIMESTAMP_TIMERS:
		g_value_set_boolean(val, self->wants_timestamp_timers);
		break;
	case PROP_TIME_FORMAT:
		g_value_set_enum(val, (int) self->time_format);
		break;
	case PROP_PLAN_TYPE:
		g_value_set_enum(val, (int) self->plan_type);
		break;
	case PROP_CLOCK:
		g_value_set_enum(val, (int) self->clock);
		break;
	case PROP_DECIMAL_SYMBOL:
		g_value_set_string(val, self->decimal_symbol);
		break;
	case PROP_THOUSANDS_OPERATOR:
		g_value_set_string(val, self->thousands_operator);
		break;
	case PROP_COLOR_SCHEME:
		g_value_set_string(val, self->color_scheme);
		break;
	case PROP_WEEKLY_CAPACITY:
		g_value_set_uint(val, self->weekly_capacity);
		break;
	case PROP_EXPENSE_FEATURE:
		g_value_set_boolean(val, self->exepnse_feature);
		break;
	case PROP_INVOICE_FEATURE:
		g_value_set_boolean(val, self->invoice_feature);
		break;
	case PROP_ESTIMATE_FEATURE:
		g_value_set_boolean(val, self->estimate_feature);
		break;
	case PROP_APPROVAL_FEATURE:
		g_value_set_boolean(val, self->approval_feature);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_company_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestCompany *self = HARVEST_COMPANY(obj);

	switch (prop_id) {
	case PROP_BASE_URI:
		g_free(self->base_uri);
		self->base_uri = g_value_dup_string(val);
		break;
	case PROP_FULL_DOMAIN:
		g_free(self->full_domain);
		self->full_domain = g_value_dup_string(val);
		break;
	case PROP_NAME:
		g_free(self->name);
		self->name = g_value_dup_string(val);
		break;
	case PROP_IS_ACTIVE:
		self->is_active = g_value_get_boolean(val);
		break;
	case PROP_WEEK_START_DAY:
		self->week_start_day = (HarvestWeekStartDay) g_value_get_enum(val);
		break;
	case PROP_WANTS_TIMESTAMP_TIMERS:
		self->wants_timestamp_timers = g_value_get_boolean(val);
		break;
	case PROP_TIME_FORMAT:
		self->time_format = (HarvestTimeFormat) g_value_get_enum(val);
		break;
	case PROP_PLAN_TYPE:
		self->plan_type = (HarvestPlanType) g_value_get_enum(val);
		break;
	case PROP_CLOCK:
		self->clock = (HarvestClock) g_value_get_enum(val);
		break;
	case PROP_DECIMAL_SYMBOL:
		g_free(self->decimal_symbol);
		self->decimal_symbol = g_value_dup_string(val);
		break;
	case PROP_THOUSANDS_OPERATOR:
		g_free(self->thousands_operator);
		self->thousands_operator = g_value_dup_string(val);
		break;
	case PROP_COLOR_SCHEME:
		g_free(self->color_scheme);
		self->color_scheme = g_value_dup_string(val);
		break;
	case PROP_WEEKLY_CAPACITY:
		self->weekly_capacity = g_value_get_uint(val);
		break;
	case PROP_EXPENSE_FEATURE:
		self->exepnse_feature = g_value_get_boolean(val);
		break;
	case PROP_INVOICE_FEATURE:
		self->invoice_feature = g_value_get_boolean(val);
		break;
	case PROP_ESTIMATE_FEATURE:
		self->estimate_feature = g_value_get_boolean(val);
		break;
	case PROP_APPROVAL_FEATURE:
		self->approval_feature = g_value_get_boolean(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_company_class_init(HarvestCompanyClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_company_finalize;
	obj_class->get_property = harvest_company_get_property;
	obj_class->set_property = harvest_company_set_property;

	obj_properties[PROP_BASE_URI]
		= g_param_spec_string("base-uri", _("Base URI"), _("The Harvest URL for the company."),
			NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_FULL_DOMAIN] = g_param_spec_string("full-domain", _("Full Domain"),
		_("The Harvest domain for the company."), NULL,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_NAME]
		= g_param_spec_string("name", _("Name"), _("The name of the company."), NULL,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_IS_ACTIVE]		= g_param_spec_boolean("is-active", _("Is Active"),
		 _("Whether the company is active or archived."), FALSE,
		 G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_WEEK_START_DAY] = g_param_spec_enum("week-start-day", _("Week Start Day"),
		_("The week day used as the start of the week. Returns one of: Saturday, Sunday, or "
		  "Monday."),
		HARVEST_TYPE_WEEK_START_DAY, HARVEST_WEEK_START_DAY_SUNDAY,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_WANTS_TIMESTAMP_TIMERS]
		= g_param_spec_boolean("wants-timestamp-timers", _("Wants Timestamp Timers"),
			_("Whether time is tracked via duration or start and end times."), FALSE,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_TIME_FORMAT]	= g_param_spec_enum("time-format", _("Time Format"),
		   _("The format used to display time in Harvest. Returns either decimal or hours_minutes."),
		   HARVEST_TYPE_TIME_FORMAT, HARVEST_TIME_FORMAT_HOURS_MINUTES,
		   G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_PLAN_TYPE]		= g_param_spec_enum("plan-type", _("Plan Type"),
		 _("The type of plan the company is on. Examples: trial, free, or simple-v4"),
		 HARVEST_TYPE_PLAN_TYPE, HARVEST_PLAN_TYPE_FREE,
		 G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_CLOCK]			= g_param_spec_enum("clock", _("Clock"),
		 _("Used to represent whether the company is using a 12-hour or 24-hour clock. Returns "
		   "either 12h or 24h."),
		 HARVEST_TYPE_CLOCK, HARVEST_CLOCK_12H,
		 G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_DECIMAL_SYMBOL] = g_param_spec_string("decimal-symbol", _("Decimal Symbol"),
		_("Symbol used when formatting decimals."), NULL,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_THOUSANDS_OPERATOR] = g_param_spec_string("thousands-operator",
		_("Thousands Operator"), _("Separator used when formatting numbers."), NULL,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_COLOR_SCHEME]		= g_param_spec_string("color-scheme", _("Color Scheme"),
		  _("The color scheme being used in the Harvest web client."), NULL,
		  G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_WEEKLY_CAPACITY]	= g_param_spec_uint("weekly-capacity",
		   _("Weekly Capacity"), _("The weekly capacity in seconds."), 0, UINT_MAX, 0,
		   G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_EXPENSE_FEATURE]	= g_param_spec_boolean("expense-feature", _(" Feature"),
		   _("Whether the expense module is enabled."), FALSE,
		   G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_INVOICE_FEATURE]	= g_param_spec_boolean("invoice-feature", _(" Feature"),
		   _("Whether the invoice module is enabled."), FALSE,
		   G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_ESTIMATE_FEATURE] = g_param_spec_boolean("estimate-feature", _(" Feature"),
		_("Whether the estimate module is enabled."), FALSE,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_APPROVAL_FEATURE] = g_param_spec_boolean("approval-feature", _(" Feature"),
		_("Whether the approval module is enabled."), FALSE,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_company_init(G_GNUC_UNUSED HarvestCompany *self)
{}

const char *
harvest_company_get_full_domain(HarvestCompany *self)
{
	return self->full_domain;
}

const char *
harvest_company_get_name(HarvestCompany *self)
{
	return self->name;
}

HarvestResponse *
harvest_company_get_company(HarvestApiClient *client)
{
	g_return_val_if_fail(HARVEST_IS_API_CLIENT(client), NULL);

	g_autoptr(HarvestGetCompanyRequest) request = harvest_get_company_request_new();

	return harvest_api_client_execute_request_sync(client, HARVEST_REQUEST(request));
}

void
harvest_company_get_company_async(
	HarvestApiClient *client, HarvestCompletedCallback *callback, gpointer user_data)
{
	HarvestGetCompanyRequest *request = harvest_get_company_request_new();
	g_signal_connect(HARVEST_REQUEST(request), "completed", G_CALLBACK(callback), user_data);

	harvest_api_client_execute_request_async(client, HARVEST_REQUEST(request));
}
