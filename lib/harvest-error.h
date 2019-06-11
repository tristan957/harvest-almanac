#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

#define PACKAGE_DOMAIN 0

typedef enum HarvestErrorCode
{
	ERROR_CURL,
	ERROR_JSON,
} TlltCpErrorCode;

G_BEGIN_DECLS

G_END_DECLS
