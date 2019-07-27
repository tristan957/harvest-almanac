#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-response.h"

struct _HarvestResponse
{
	GObject parent_instance;

	GType body_type;
};

G_DEFINE_TYPE(HarvestResponse, harvest_response, G_TYPE_OBJECT)

enum HarvestResponseProps
{
	PROP_0,
	PROP_BODY_TYPE,
	N_PROPS
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_response_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestResponse *self = HARVEST_RESPONSE(obj);

	switch (prop_id) {
	case PROP_BODY_TYPE:
		g_value_set_uint64(val, self->body_type);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_response_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestResponse *self = HARVEST_RESPONSE(obj);

	switch (prop_id) {
	case PROP_BODY_TYPE:
		self->body_type = g_value_get_uint64(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_response_class_init(HarvestResponseClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->get_property = harvest_response_get_property;
	obj_class->set_property = harvest_response_set_property;

	obj_properties[PROP_BODY_TYPE] = g_param_spec_uint64("body-type", _("Body Type"),
		_("The GType of the response body."), G_TYPE_NONE, __UINT64_MAX__, G_TYPE_NONE,
		G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE | G_PARAM_WRITABLE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_response_init(G_GNUC_UNUSED HarvestResponse *self)
{}

HarvestResponse *
harvest_response_new(const GType body_type)
{
	return g_object_new(HARVEST_TYPE_RESPONSE, "body-type", body_type);
}

GType
harvest_response_get_body_type(HarvestResponse *self)
{
	return self->body_type;
}
