#include "config.h"

#include <locale.h>

#include <glib-object.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <handy.h>

#include "hal-application.h"
#include "hal-resources.h"

int
main(int argc, char *argv[])
{
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	textdomain(GETTEXT_PACKAGE);
	g_resources_register(hal_get_resource());

	if (!hdy_init(&argc, &argv)) {
		g_critical("Unable to initialize libhandy");

		return 1;
	}

	g_autoptr(HalApplication) app = hal_application_new("io.partin.tristan.HarvestAlmanac");

	g_set_application_name(_("Harvest Almanac"));

	return g_application_run(G_APPLICATION(app), argc, argv);
}
