#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

#include "harvest-glib/http/request/harvest-request.h"

G_BEGIN_DECLS

#define HARVEST_TYPE_GET_COMPANY_REQUEST (harvest_get_company_request_get_type())
G_DECLARE_FINAL_TYPE(HarvestGetCompanyRequest, harvest_get_company_request, HARVEST,
	GET_COMPANY_REQUEST, HarvestRequest)

HarvestGetCompanyRequest *harvest_get_company_request_new(void) G_GNUC_WARN_UNUSED_RESULT;

G_END_DECLS
