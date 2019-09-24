#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum HarvestErrorCode
{
	ERROR_MEM,
	ERROR_JSON,
} HarvestCpErrorCode;

G_DEFINE_QUARK("libharvest-glib", harvest_error)

G_END_DECLS
