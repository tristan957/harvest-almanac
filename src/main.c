#include <glib-object.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

// We have to set the domain here in order to avoid a macro redefinition
#ifdef G_LOG_DOMAIN
#	undef G_LOG_DOMAIN
#	define G_LOG_DOMAIN "Harvest Almanac"
#endif

#include "hal-application.h"
#include "hal-config.h"

int
main(int argc, char *argv[])
{
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);

	g_autoptr(HalApplication) app = hal_application_new("io.partin.tristan.HarvestAlmanac");

	g_set_application_name(_("Harvest Almanac"));

	return g_application_run(G_APPLICATION(app), argc, argv);
}
