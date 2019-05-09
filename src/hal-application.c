#include <glib-object.h>
#include <gtk/gtk.h>

#include "hal-application.h"
#include "hal-config.h"
#include "hal-resources.h"
#include "hal-window.h"

struct _HalApplication
{
	GtkApplication parent_instance;
};

typedef struct HalApplicationPrivate
{
	HalWindow *main_window;
} HalApplicationPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HalApplication, hal_application, GTK_TYPE_APPLICATION)

static void
hal_application_activate(GApplication *self)
{
	HalApplicationPrivate *priv = hal_application_get_instance_private(HAL_APPLICATION(self));

	if (priv->main_window == NULL) {
		priv->main_window = hal_window_new(self);
	}

	gtk_window_present(GTK_WINDOW(priv->main_window));
}

static void
hal_application_about(G_GNUC_UNUSED GSimpleAction *action, G_GNUC_UNUSED GVariant *param,
					  gpointer data)
{
	static const char *authors[] = {"Tristan Partin"};

	HalApplication *self		= HAL_APPLICATION(data);
	HalApplicationPrivate *priv = hal_application_get_instance_private(self);

	gtk_show_about_dialog(GTK_WINDOW(priv->main_window), "program-name", PACKAGE_NAME, "version",
						  PACKAGE_VERSION, "license-type", PACKAGE_LICENSE, "website",
						  PACKAGE_WEBSITE, "website-label", PACKAGE_WEBSITE_LABEL, "authors",
						  authors, "logo-icon-name", "trophy-gold", NULL);
}

static void
hal_application_startup(GApplication *self)
{
	g_resources_register(hal_get_resource());
	g_application_set_resource_base_path(self, "/io/partin/tristan/HarvestAlmanac");

	G_APPLICATION_CLASS(hal_application_parent_class)->startup(self);
}

static void
hal_application_finalize(GObject *obj)
{
	G_OBJECT_CLASS(hal_application_parent_class)->finalize(obj);
}

static void
hal_application_class_init(HalApplicationClass *klass)
{
	GObjectClass *obj_class		 = G_OBJECT_CLASS(klass);
	GApplicationClass *app_class = G_APPLICATION_CLASS(klass);

	obj_class->finalize = hal_application_finalize;
	app_class->activate = hal_application_activate;
	app_class->startup  = hal_application_startup;
}

static const GActionEntry app_entries[] = {{.name = "about", .activate = hal_application_about}};

static void
hal_application_init(HalApplication *self)
{
	g_action_map_add_action_entries(G_ACTION_MAP(self), app_entries, G_N_ELEMENTS(app_entries),
									self);
}

HalApplication *
hal_application_new(const char *id)
{
	return g_object_new(HAL_TYPE_APPLICATION, "application-id", id, NULL);
}
