#include <glib-object.h>
#include <glib/gi18n.h>

#include "harvest-response-metadata.h"
#include "shared/harvest-http.h"

struct _HarvestResponseMetadata
{
	GObject parent_instance;

	HttpStatusCode expected_status;
	GType body_type;
};

G_DEFINE_TYPE(HarvestResponseMetadata, harvest_response_metadata, G_TYPE_OBJECT)

enum HarvestResponseMetadataProps
{
	PROP_0,
	PROP_EXPECTED_STATUS,
	PROP_BODY_TYPE,
	N_PROPS
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_response_metadata_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestResponseMetadata *self = HARVEST_RESPONSE_METADATA(obj);

	switch (prop_id) {
	case PROP_EXPECTED_STATUS:
		g_value_set_int(val, self->expected_status);
		break;
	case PROP_BODY_TYPE:
		g_value_set_uint64(val, self->body_type);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_response_metadata_set_property(
	GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestResponseMetadata *self = HARVEST_RESPONSE_METADATA(obj);

	switch (prop_id) {
	case PROP_EXPECTED_STATUS:
		self->expected_status = g_value_get_int(val);
		break;
	case PROP_BODY_TYPE:
		self->body_type = g_value_get_uint64(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_response_metadata_class_init(HarvestResponseMetadataClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->get_property = harvest_response_metadata_get_property;
	obj_class->set_property = harvest_response_metadata_set_property;

	obj_properties[PROP_EXPECTED_STATUS] = g_param_spec_int("expected-status", _("Expected Status"),
		_("The expected status code the response should come back with."), HTTP_STATUS_OK,
		HTTP_STATUS_GATEWAY_TIMEOUT, HTTP_STATUS_OK,
		G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE);
	obj_properties[PROP_BODY_TYPE]		 = g_param_spec_uint64("body-type", _("Body Type"),
		  _("The GType of the response body."), G_TYPE_NONE, __UINT64_MAX__, G_TYPE_NONE,
		  G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE | G_PARAM_WRITABLE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_response_metadata_init(G_GNUC_UNUSED HarvestResponseMetadata *self)
{}

HarvestResponseMetadata *
harvest_response_metadata_new(const GType body_type, const HttpStatusCode expected_status)
{
	return g_object_new(
		HARVEST_TYPE_RESPONSE_METADATA, "body-type", body_type, "expected-status", expected_status);
}

HttpStatusCode
harvest_response_metadata_get_expected_status(HarvestResponseMetadata *self)
{
	return self->expected_status;
}

GType
harvest_response_metadata_get_body_type(HarvestResponseMetadata *self)
{
	return self->body_type;
}