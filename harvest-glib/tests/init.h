#pragma once

#include <stdlib.h>

#include <json-glib/json-glib.h>
#include <libsoup/soup.h>

#include "harvest-glib/harvest.h"

HarvestApiClient *
initialize_environment(void)
{
	char **env				  = g_get_environ();
	const char *access_token  = g_environ_getenv(env, "HARVEST_API_ACCESS_TOKEN");
	const char *contact_email = g_environ_getenv(env, "HARVEST_API_CONTACT_EMAIL");
	const char *account_id	  = g_environ_getenv(env, "HARVEST_API_ACCOUNT_ID");
	const int max_connections = atoi(g_environ_getenv(env, "HARVEST_SOUP_MAX_CONNECTIONS"));
	const int logger_level	  = atoi(g_environ_getenv(env, "HARVEST_SOUP_LOG_LEVEL"));

	g_autoptr(SoupLogger) logger = soup_logger_new(logger_level, -1);

	g_autofree char *user_agent = g_strdup_printf("Harvest Almanac (%s)", contact_email);

	g_autoptr(SoupSession) session = soup_session_new_with_options(SOUP_SESSION_MAX_CONNS,
		max_connections, SOUP_SESSION_USER_AGENT, user_agent, SOUP_SESSION_ADD_FEATURE_BY_TYPE,
		SOUP_TYPE_CONTENT_SNIFFER, SOUP_SESSION_ADD_FEATURE, SOUP_SESSION_FEATURE(logger), NULL);

	return harvest_api_client_new(session, access_token, account_id);
}
