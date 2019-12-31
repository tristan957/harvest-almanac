#include <glib.h>
#include <libsoup/soup.h>

#include "harvest.h"
#include "init.h"

static void
test_harvest_get_me(void)
{
	HarvestResponse *res = harvest_user_get_me();

	g_assert_null(res->err);
}

int
main(int argc, char **argv)
{
	g_test_init(&argc, &argv, NULL);

	initialize_environment();

	g_test_add_func("/user/me", test_harvest_get_me);

	return g_test_run();
}
