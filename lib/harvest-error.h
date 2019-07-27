#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define PACKAGE_DOMAIN 0

typedef enum HarvestErrorCode
{
	ERROR_CURL,
	ERROR_JSON,
} HarvestCpErrorCode;

G_END_DECLS
