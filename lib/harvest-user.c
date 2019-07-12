#include <float.h>
#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>
#include <json-glib/json-glib.h>

#include "harvest-user.h"

struct _HarvestUser
{
	int id;
	char *first_name;
	char *last_name;
	char *email;
	char *telephone;
	char *timezone;
	gboolean has_access_to_all_future_projects : 1;
	gboolean is_contractor : 1;
	gboolean is_admin : 1;
	gboolean is_project_manager : 1;
	gboolean can_see_rates : 1;
	gboolean can_create_projects : 1;
	gboolean can_create_invoices : 1;
	gboolean is_active : 1;
	int weekly_capacity;
	double default_hourly_rate;
	double cost_rate;
	JsonArray *roles;
	unsigned int num_roles;
	char *avatar_url;
	GDateTime *created_at;
	GDateTime *updated_at;
};

G_DEFINE_TYPE(HarvestUser, harvest_user, G_TYPE_OBJECT)

enum HarvestUserProps
{
	PROP_0,
	PROP_ID,
	PROP_FIRST_NAME,
	PROP_LAST_NAME,
	PROP_EMAIL,
	PROP_TELEPHONE,
	PROP_TIMEZONE,
	PROP_HAS_ACCESS_TO_ALL_FUTURE_PROJECTS,
	PROP_IS_CONTRACTOR,
	PROP_IS_ADMIN,
	PROP_IS_PROJECT_MANAGER,
	PROP_CAN_SEE_RATES,
	PROP_CAN_CREATE_PROJECTS,
	PROP_CAN_CREATE_INVOICES,
	PROP_IS_ACTIVE,
	PROP_WEEKLY_CAPACITY,
	PROP_DEFAULT_HOURLY_RATE,
	PROP_COST_RATE,
	PROP_ROLES,
	PROP_AVATAR_URL,
	PROP_CREATED_AT,
	PROP_UPDATED_AT,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_user_finalize(GObject *obj)
{
	HarvestUser *self = HARVEST_USER(obj);

	g_free(self->first_name);
	g_free(self->last_name);
	g_free(self->email);
	g_free(self->email);
	g_free(self->telephone);
	g_free(self->timezone);
	g_free(self->avatar_url);

	G_OBJECT_CLASS(harvest_user_parent_class)->finalize(obj);
}

static void
harvest_user_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestUser *self = HARVEST_USER(obj);

	switch (prop_id) {
	case PROP_ID:
		g_value_set_int(val, self->id);
		break;
	case PROP_FIRST_NAME:
		g_value_set_string(val, self->first_name);
		break;
	case PROP_LAST_NAME:
		g_value_set_string(val, self->last_name);
		break;
	case PROP_EMAIL:
		g_value_set_string(val, self->email);
		break;
	case PROP_TELEPHONE:
		g_value_set_string(val, self->telephone);
		break;
	case PROP_TIMEZONE:
		g_value_set_string(val, self->telephone);
		break;
	case PROP_HAS_ACCESS_TO_ALL_FUTURE_PROJECTS:
		g_value_set_boolean(val, self->has_access_to_all_future_projects);
		break;
	case PROP_IS_CONTRACTOR:
		g_value_set_boolean(val, self->is_contractor);
		break;
	case PROP_IS_ADMIN:
		g_value_set_boolean(val, self->is_admin);
		break;
	case PROP_IS_PROJECT_MANAGER:
		g_value_set_boolean(val, self->is_contractor);
		break;
	case PROP_CAN_SEE_RATES:
		g_value_set_boolean(val, self->is_contractor);
		break;
	case PROP_CAN_CREATE_PROJECTS:
		g_value_set_boolean(val, self->is_contractor);
		break;
	case PROP_CAN_CREATE_INVOICES:
		g_value_set_boolean(val, self->is_contractor);
		break;
	case PROP_IS_ACTIVE:
		g_value_set_boolean(val, self->is_contractor);
		break;
	case PROP_WEEKLY_CAPACITY:
		g_value_set_double(val, self->weekly_capacity);
		break;
	case PROP_DEFAULT_HOURLY_RATE:
		g_value_set_double(val, self->default_hourly_rate);
		break;
	case PROP_COST_RATE:
		g_value_set_double(val, self->cost_rate);
		break;
	case PROP_ROLES:
		g_value_set_object(val, self->roles);
		break;
	case PROP_AVATAR_URL:
		g_value_set_string(val, self->avatar_url);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_user_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestUser *self = HARVEST_USER(obj);

	switch (prop_id) {
	case PROP_ID:
		self->id = g_value_get_int(val);
		break;
	case PROP_FIRST_NAME:
		g_free(self->first_name);
		self->first_name = g_value_dup_string(val);
		break;
	case PROP_LAST_NAME:
		g_free(self->last_name);
		self->last_name = g_value_dup_string(val);
		break;
	case PROP_EMAIL:
		g_free(self->email);
		self->email = g_value_dup_string(val);
		break;
	case PROP_TELEPHONE:
		g_free(self->telephone);
		self->telephone = g_value_dup_string(val);
		break;
	case PROP_TIMEZONE:
		g_free(self->timezone);
		self->timezone = g_value_dup_string(val);
		break;
	case PROP_HAS_ACCESS_TO_ALL_FUTURE_PROJECTS:
		self->has_access_to_all_future_projects = g_value_get_boolean(val);
		break;
	case PROP_IS_CONTRACTOR:
		self->is_contractor = g_value_get_boolean(val);
		break;
	case PROP_IS_ADMIN:
		self->is_admin = g_value_get_boolean(val);
		break;
	case PROP_IS_PROJECT_MANAGER:
		self->is_project_manager = g_value_get_boolean(val);
		break;
	case PROP_CAN_SEE_RATES:
		self->can_see_rates = g_value_get_boolean(val);
		break;
	case PROP_CAN_CREATE_PROJECTS:
		self->can_create_projects = g_value_get_boolean(val);
		break;
	case PROP_CAN_CREATE_INVOICES:
		self->can_create_projects = g_value_get_boolean(val);
		break;
	case PROP_IS_ACTIVE:
		self->is_active = g_value_get_boolean(val);
		break;
	case PROP_WEEKLY_CAPACITY:
		self->weekly_capacity = g_value_get_double(val);
		break;
	case PROP_DEFAULT_HOURLY_RATE:
		self->default_hourly_rate = g_value_get_double(val);
		break;
	case PROP_COST_RATE:
		self->cost_rate = g_value_get_double(val);
		break;
	case PROP_ROLES:
		g_object_unref(self->roles);
		self->roles = g_value_get_object(val);
		break;
	case PROP_AVATAR_URL:
		g_free(self->avatar_url);
		self->avatar_url = g_value_dup_string(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_user_class_init(HarvestUserClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_user_finalize;
	obj_class->get_property = harvest_user_get_property;
	obj_class->set_property = harvest_user_set_property;

	obj_properties[PROP_ID] =
		g_param_spec_int("id", _("ID"), _("User ID"), 0, INT_MAX, 0, G_PARAM_READWRITE);
	obj_properties[PROP_FIRST_NAME] = g_param_spec_string("first-name", _("First Name"),
		_("The first name of the user."), NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_LAST_NAME]  = g_param_spec_string("last-name", _("Last Name"),
		 _("The last name of the user."), NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_EMAIL]		= g_param_spec_string("email", _("Email"),
		 _("The email address of the user."), NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_TELEPHONE]  = g_param_spec_string("telephone", _("Telephone"),
		 _("The telephone number for the user."), NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_TIMEZONE]   = g_param_spec_string("timezone", _("Timezone"),
		  _("The user’s timezone."), NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_HAS_ACCESS_TO_ALL_FUTURE_PROJECTS] = g_param_spec_boolean(
		"has-access-to-all-future-projects", _("Has Access to All Future Projects"),
		_(" 	Whether the user should be automatically added to future projects."), FALSE,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_IS_CONTRACTOR] = g_param_spec_boolean("is-contractor", _("Is Contractor"),
		_("Whether the user is a contractor or an employee."), FALSE,
		G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_IS_ADMIN]	  = g_param_spec_boolean("is-admin", _("Is Admin"),
		 _("Whether the user has admin permissions."), FALSE, G_PARAM_READABLE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_IS_PROJECT_MANAGER] = g_param_spec_boolean("is-project-manager",
		_("Is Project Manager"), _("Whether the user has project manager permissions."), FALSE,
		G_PARAM_READABLE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_CAN_SEE_RATES] = g_param_spec_boolean("can-see-rates", _("Can See Rates"),
		_("Whether the user can see billable rates on projects. Only applicable to project "
		  "managers."),
		FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_CAN_CREATE_PROJECTS] =
		g_param_spec_boolean("can-create-projects", _("Can Create Projects"),
			_("Whether the user can create projects. Only applicable to project managers."), FALSE,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_CAN_CREATE_INVOICES] =
		g_param_spec_boolean("can-create-invoices", _("Can Create Invoices"),
			_("Whether the user can create invoices. Only applicable to project managers."), FALSE,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_IS_ACTIVE] = g_param_spec_boolean("is-active", _("Is Active"),
		_("Whether the user is active or archived."), FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_WEEKLY_CAPACITY] =
		g_param_spec_double("weekly-capacity", _("Weekly Capacity"),
			_("The number of hours per week this person is available to work in seconds, in half "
			  "hour increments. For example, if a person’s capacity is 35 hours, the API will "
			  "return 126000 seconds."),
			0, DBL_MAX, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_DEFAULT_HOURLY_RATE] =
		g_param_spec_double("default-hourly-rate", _("Default Hourly Rate"),
			_("The billable rate to use for this user when they are added to a project."), 0,
			DBL_MAX, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_COST_RATE] = g_param_spec_double("cost-rate", _("Cost Rate"),
		_("The cost rate to use for this user when calculating a project’s "
		  "costs vs billable amount."),
		0, DBL_MAX, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_ROLES] =
		g_param_spec_object("roles", _("Roles"), _("The role names assigned to this person."),
			JSON_TYPE_ARRAY, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_AVATAR_URL] = g_param_spec_string("avatar-url", _("Avatar URL"),
		_("The URL to the user’s avatar image."), NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_user_init(G_GNUC_UNUSED HarvestUser *self)
{}
