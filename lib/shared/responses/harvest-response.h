#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define HARVEST_TYPE_RESPONSE (harvest_response_get_type())
G_DECLARE_FINAL_TYPE(HarvestResponse, harvest_response, HARVEST, RESPONSE, GObject)

HarvestResponse *harvest_response_new(const GType body_type);
GType harvest_response_get_body_type(HarvestResponse *self) G_GNUC_CONST;

G_END_DECLS
