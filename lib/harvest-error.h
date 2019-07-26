#pragma once

#include <glib-object.h>

#define PACKAGE_DOMAIN 0

typedef enum HarvestErrorCode
{
	ERROR_CURL,
	ERROR_JSON,
} TlltCpErrorCode;

G_BEGIN_DECLS

G_END_DECLS
