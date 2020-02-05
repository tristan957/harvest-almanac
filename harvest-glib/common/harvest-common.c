#include "harvest-glib/config.h"

#include <glib-object.h>

#include "harvest-glib/common/harvest-common.h"

// Never gets unrefed
static const GRegex *regex;
static const unsigned int HARVEST_DATE_EXPECTED_MATCHES = 4;

GDateTime *
g_date_time_new_from_abbreviated_date(const char *date)
{
	g_return_val_if_fail(date != NULL, NULL);

	// Compile once to save time
	if (regex == NULL)
		regex = g_regex_new("(\\d+)-(\\d){1,2}-(\\d){1,2}", 0, 0, NULL);

	GMatchInfo *match_info;
	const gboolean found = g_regex_match(regex, date, 0, &match_info);
	if (!found || match_info == NULL)
		return NULL;

	gchar **matches = g_match_info_fetch_all(match_info);
	if (matches == NULL || g_match_info_get_match_count(match_info) != HARVEST_DATE_EXPECTED_MATCHES
		|| matches[1] == NULL || matches[2] == NULL || matches[3] == NULL)
		return NULL;

	GDateTime *dt
		= g_date_time_new(NULL, atoi(matches[1]), atoi(matches[2]), atoi(matches[3]), 0, 0, 0);

	g_strfreev(matches);
	g_match_info_unref(match_info);

	return dt;
}
