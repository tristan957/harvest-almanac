#include "config.h"
#define G_LOG_DOMAIN "HarvestLinks"

#include <glib-object.h>
#include <glib/gi18n-lib.h>

#include "harvest-links.h"

struct _HarvestLinks
{
	char *first;
	char *next;
	char *previous;
	char *last;
};

G_DEFINE_TYPE(HarvestLinks, harvest_links, G_TYPE_OBJECT)

enum HarvestLinksProps
{
	PROP_0,
	PROP_FIRST,
	PROP_NEXT,
	PROP_PREVIOUS,
	PROP_LAST,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_links_finalize(GObject *obj)
{
	HarvestLinks *self = HARVEST_LINKS(obj);

	g_free(self->first);
	g_free(self->next);
	g_free(self->previous);
	g_free(self->last);

	G_OBJECT_CLASS(harvest_links_parent_class)->finalize(obj);
}

static void
harvest_links_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestLinks *self = HARVEST_LINKS(obj);

	switch (prop_id) {
	case PROP_FIRST:
		g_value_set_string(val, self->first);
		break;
	case PROP_NEXT:
		g_value_set_string(val, self->next);
		break;
	case PROP_PREVIOUS:
		g_value_set_string(val, self->previous);
		break;
	case PROP_LAST:
		g_value_set_string(val, self->last);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_links_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestLinks *self = HARVEST_LINKS(obj);

	switch (prop_id) {
	case PROP_FIRST:
		g_free(self->first);
		self->first = g_value_dup_string(val);
		break;
	case PROP_NEXT:
		g_free(self->next);
		self->next = g_value_dup_string(val);
		break;
	case PROP_PREVIOUS:
		g_free(self->previous);
		self->previous = g_value_dup_string(val);
		break;
	case PROP_LAST:
		g_free(self->last);
		self->last = g_value_dup_string(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_links_class_init(HarvestLinksClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_links_finalize;
	obj_class->get_property = harvest_links_get_property;
	obj_class->set_property = harvest_links_set_property;

	obj_properties[PROP_FIRST]
		= g_param_spec_string("first", _("First"), _("Link to the first page of results."), NULL,
			G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_NEXT]
		= g_param_spec_string("next", _("Next"), _("Link to the next page of results."), NULL,
			G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_PREVIOUS]
		= g_param_spec_string("previous", _("Previous"), _("Link to the previous page of results."),
			NULL, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_LAST]
		= g_param_spec_string("last", _("Last"), _("Link to the last page of results."), NULL,
			G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_links_init(G_GNUC_UNUSED HarvestLinks *self)
{}
