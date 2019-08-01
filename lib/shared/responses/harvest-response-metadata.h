#pragma once

#include <glib-object.h>

#include "shared/harvest-http.h"

G_BEGIN_DECLS

#define HARVEST_TYPE_RESPONSE_METADATA (harvest_response_metadata_get_type())
G_DECLARE_FINAL_TYPE(
	HarvestResponseMetadata, harvest_response_metadata, HARVEST, RESPONSE_METADATA, GObject)

HarvestResponseMetadata *harvest_response_metadata_new(
	const GType body_type, const HttpStatusCode expected_status);
HttpStatusCode harvest_response_metadata_get_expected_status(
	HarvestResponseMetadata *self) G_GNUC_CONST;
GType harvest_response_metadata_get_body_type(HarvestResponseMetadata *self) G_GNUC_CONST;

G_END_DECLS
