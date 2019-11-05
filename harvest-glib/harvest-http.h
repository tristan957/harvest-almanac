#pragma once

#if !defined(__HARVEST_HEADER_INTERNAL__) && !defined(__HARVEST_COMPILATION__)
#	error "Only <harvest-glib/harvest.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum HttpMethod
{
	HTTP_METHOD_GET,
	HTTP_METHOD_POST,
	HTTP_METHOD_PATCH,
	HTTP_METHOD_DELETE,
} HttpMethod;

typedef enum HttpStatusCode
{
	HTTP_STATUS_OK					  = 200,
	HTTP_STATUS_CREATED				  = 201,
	HTTP_STATUS_FORBIDDEN			  = 403,
	HTTP_STATUS_NOT_FOUND			  = 404,
	HTTP_STATUS_UNPROCESSABLE		  = 422,
	HTTP_STATUS_TOO_MANY_REQUESTS	  = 429,
	HTTP_STATUS_INTERNAL_SERVER_ERROR = 500,
} HttpStatusCode;

G_END_DECLS
