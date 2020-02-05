#include "harvest-glib/config.h"

#include <glib-object.h>
#include <glib/gi18n-lib.h>

#include "harvest-glib/http/response/harvest-links.h"
#include "harvest-glib/http/response/harvest-paged.h"

typedef struct _HarvestPagedPrivate
{
	GObject parent_instance;

	unsigned int per_page;
	unsigned int total_pages;
	unsigned int total_entries;
	char *next_page;
	char *previous_page;
	unsigned int page;
	HarvestLinks *links;
} HarvestPagedPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HarvestPaged, harvest_paged, G_TYPE_OBJECT)

enum HarvestPagedProps
{
	PROP_0,
	PROP_PER_PAGE,
	PROP_TOTAL_PAGES,
	PROP_TOTAL_ENTRIES,
	PROP_NEXT_PAGE,
	PROP_PREVIOUS_PAGE,
	PROP_PAGE,
	PROP_LINKS,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
harvest_paged_finalize(GObject *obj)
{
	HarvestPaged *self		  = HARVEST_PAGED(obj);
	HarvestPagedPrivate *priv = harvest_paged_get_instance_private(self);

	g_free(priv->next_page);
	g_free(priv->previous_page);
	if (priv->links != NULL)
		g_object_unref(priv->links);

	G_OBJECT_CLASS(harvest_paged_parent_class)->finalize(obj);
}

static void
harvest_paged_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	HarvestPaged *self		  = HARVEST_PAGED(obj);
	HarvestPagedPrivate *priv = harvest_paged_get_instance_private(self);

	switch (prop_id) {
	case PROP_PER_PAGE:
		g_value_set_uint(val, priv->per_page);
		break;
	case PROP_TOTAL_PAGES:
		g_value_set_uint(val, priv->total_pages);
		break;
	case PROP_TOTAL_ENTRIES:
		g_value_set_uint(val, priv->total_entries);
		break;
	case PROP_NEXT_PAGE:
		g_value_set_string(val, priv->next_page);
		break;
	case PROP_PREVIOUS_PAGE:
		g_value_set_string(val, priv->previous_page);
		break;
	case PROP_PAGE:
		g_value_set_uint(val, priv->page);
		break;
	case PROP_LINKS:
		g_value_set_object(val, priv->links);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_paged_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	HarvestPaged *self		  = HARVEST_PAGED(obj);
	HarvestPagedPrivate *priv = harvest_paged_get_instance_private(self);

	switch (prop_id) {
	case PROP_PER_PAGE:
		priv->per_page = g_value_get_uint(val);
		break;
	case PROP_TOTAL_PAGES:
		priv->total_pages = g_value_get_uint(val);
		break;
	case PROP_TOTAL_ENTRIES:
		priv->total_entries = g_value_get_uint(val);
		break;
	case PROP_NEXT_PAGE:
		g_free(priv->next_page);
		priv->next_page = g_value_dup_string(val);
		break;
	case PROP_PREVIOUS_PAGE:
		g_free(priv->next_page);
		priv->next_page = g_value_dup_string(val);
		break;
	case PROP_PAGE:
		priv->page = g_value_get_uint(val);
		break;
	case PROP_LINKS:
		if (priv->links != NULL)
			g_object_unref(priv->links);
		priv->links = g_value_dup_object(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
harvest_paged_class_init(HarvestPagedClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= harvest_paged_finalize;
	obj_class->get_property = harvest_paged_get_property;
	obj_class->set_property = harvest_paged_set_property;

	obj_properties[PROP_PER_PAGE]
		= g_param_spec_uint("per_page", _("Per Page"), _("Number of results per page."), 0,
			UINT_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_TOTAL_PAGES]
		= g_param_spec_uint("total_pages", _("Total Pages"), _("Total number of pages of results."),
			0, UINT_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_TOTAL_ENTRIES] = g_param_spec_uint("total_entries", _("Total Entries"),
		_("Total number of entries to read."), 0, UINT_MAX, 0,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_NEXT_PAGE]
		= g_param_spec_string("next_page", _("Next Page"), _("URL of the next page of results."),
			NULL, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_PREVIOUS_PAGE] = g_param_spec_string("previous_page", _("Previous Page"),
		_("URL of the previous page of results."), NULL,
		G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_PAGE] = g_param_spec_uint("page", _("Page"), _("Current page number."), 0,
		UINT_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_LINKS]
		= g_param_spec_object("links", _("Links"), _("Links to a few other result pages."),
			HARVEST_TYPE_LINKS, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
harvest_paged_init(G_GNUC_UNUSED HarvestPaged *self)
{}
