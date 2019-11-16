#include "config.h"

#define G_LOG_DOMAIN "HarvestAlmanac"

#include <locale.h>

#include <glib-object.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <handy.h>

#include "hal-application.h"

int
main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);

	const gboolean success = hdy_init(&argc, &argv);
	if (!success) {
		g_critical("Unable to initialize libhandy");
		return 1;
	}

	g_autoptr(HalApplication) app = hal_application_new("io.partin.tristan.HarvestAlmanac");

	g_set_application_name(_("Harvest Almanac"));

	const int status = g_application_run(G_APPLICATION(app), argc, argv);

	return status;
}
