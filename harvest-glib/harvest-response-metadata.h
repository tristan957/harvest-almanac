#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

#include "harvest-http.h"

G_BEGIN_DECLS

#define HARVEST_TYPE_RESPONSE_METADATA (harvest_response_metadata_get_type())
G_DECLARE_FINAL_TYPE(
	HarvestResponseMetadata, harvest_response_metadata, HARVEST, RESPONSE_METADATA, GObject)

HarvestResponseMetadata *harvest_response_metadata_new(
	GType body_type, HttpStatusCode expected_status) G_GNUC_WARN_UNUSED_RESULT;
HttpStatusCode harvest_response_metadata_get_expected_status(
	HarvestResponseMetadata *self) G_GNUC_CONST;
GType harvest_response_metadata_get_body_type(HarvestResponseMetadata *self) G_GNUC_CONST;

G_END_DECLS
