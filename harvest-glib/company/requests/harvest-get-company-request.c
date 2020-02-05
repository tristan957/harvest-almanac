#include "harvest-glib/config.h"

#include <glib-object.h>

#include "harvest-glib/company/harvest-company.h"
#include "harvest-glib/company/requests/harvest-get-company-request.h"
#include "harvest-glib/http/harvest-http.h"
#include "harvest-glib/http/request/harvest-request.h"
#include "harvest-glib/http/response/harvest-response-metadata.h"

struct _HarvestGetCompanyRequest
{
	HarvestRequest parent_instance;
};

G_DEFINE_TYPE(HarvestGetCompanyRequest, harvest_get_company_request, HARVEST_TYPE_REQUEST)

static void
harvest_get_company_request_class_init(G_GNUC_UNUSED HarvestGetCompanyRequestClass *klass)
{}

static void
harvest_get_company_request_init(G_GNUC_UNUSED HarvestGetCompanyRequest *self)
{}

HarvestGetCompanyRequest *
harvest_get_company_request_new()
{
	g_autofree char *endpoint = g_strdup("/company");
	g_autoptr(HarvestResponseMetadata) response_metadata
		= harvest_response_metadata_new(HARVEST_TYPE_COMPANY, HTTP_STATUS_OK);

	return g_object_new(HARVEST_TYPE_GET_COMPANY_REQUEST, "http-method", HTTP_METHOD_GET,
		"endpoint", endpoint, "response-metadata", response_metadata, NULL);
}
