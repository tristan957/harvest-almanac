#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum HttpMethod
{
	HTTP_METHOD_GET,
	HTTP_METHOD_POST,
	HTTP_METHOD_UPDATE,
	HTTP_METHOD_PATCH,
	HTTP_METHOD_DELETE,
} HttpMethod;

typedef enum HttpStatusCode
{
	HTTP_STATUS_OK					   = 200,
	HTTP_STATUS_CREATED				   = 201,
	HTTP_STATUS_ACCEPTED			   = 202,
	HTTP_STATUS_BAD_REQUEST			   = 400,
	HTTP_STATUS_UNAUTHORIZED		   = 401,
	HTTP_STATUS_FORBIDDEN			   = 403,
	HTTP_STATUS_NOT_FOUND			   = 404,
	HTTP_STATUS_METHOD_NOT_ALLOWED	 = 405,
	HTTP_STATUS_NOT_ACCEPTABLE		   = 406,
	HTTP_STATUS_REQUEST_TIMEOUT		   = 408,
	HTTP_STATUS_CONFLICT			   = 409,
	HTTP_STATUS_GONE				   = 410,
	HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE = 415,
	HTTP_STATUS_TOO_MANY_REQUESTS	  = 429,
	HTTP_STATUS_INTERNAL_SERVER_ERROR  = 500,
	HTTP_STATUS_NOT_IMPLEMENTED		   = 501,
	HTTP_STATUS_BAD_GATEWAY			   = 502,
	HTTP_STATUS_SERVICE_UNAVAILABLE	= 503,
	HTTP_STATUS_GATEWAY_TIMEOUT		   = 504,
} HttpStatusCode;

G_END_DECLS
