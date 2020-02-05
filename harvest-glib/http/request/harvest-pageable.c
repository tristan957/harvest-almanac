#include "harvest-glib/config.h"

#include <glib-object.h>
#include <glib/gi18n-lib.h>

#include "harvest-glib/http/request/harvest-pageable.h"

G_DEFINE_INTERFACE(HarvestPageable, harvest_pageable, G_TYPE_OBJECT)

static void
harvest_pageable_default_init(G_GNUC_UNUSED HarvestPageableInterface *iface)
{
	g_object_interface_install_property(
		iface, g_param_spec_uint("page", _("Page"), _("Page number of the results to retrieve."), 1,
				   UINT_MAX, 1, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
	g_object_interface_install_property(iface,
		g_param_spec_uint("per_page", _("Per Page"), _("Number of results to return per page."), 0,
			UINT_MAX, 0, G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}
