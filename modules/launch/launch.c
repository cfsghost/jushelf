#include <glib.h>
#include <glib/gi18n.h>
#include <clutter/clutter.h>
#include <jushelf/module.h>
#include <jushelf/conf.h>
#include <jushelf/widget.h>

typedef struct {
	ClutterActor *container;
	ClutterActor *icon_actor;
	gchar *icon;
	gchar *name;
	gchar *exec;
} Launch;

static gboolean
launch_clicked_cb(Launch *launch)
{
	g_print("Execute \"%s\"\n", launch->exec);
	g_spawn_command_line_async(launch->exec, NULL);
	return TRUE;
}

static void
launch_constructor(JshWidget *widget)
{
	Launch *launch;

	if (widget->priv) {
		launch = (Launch *)widget->priv;
	} else {
		launch = g_slice_new0(Launch);
		launch->name = g_strdup("Default");
		widget->priv = (gpointer)launch;
	}

	/* Initializing object */
	launch->container = clutter_group_new();
	launch->icon_actor = clutter_texture_new_from_file(launch->icon, NULL);
	clutter_container_add_actor(CLUTTER_CONTAINER(launch->container), launch->icon_actor);
	clutter_container_add_actor(CLUTTER_CONTAINER(widget->container), launch->container);

	/* Set behavior */
	clutter_actor_set_reactive(launch->container, TRUE);
	g_signal_connect_swapped(launch->container, "button-press-event",
		G_CALLBACK(launch_clicked_cb), launch);

	g_print("Constructor\n");
}

static void
launch_config(JshWidget *widget, JsonNode *node)
{
	Launch *launch;
	JsonObject *object = json_node_get_object(node);

	g_print("Read config\n");

	/* Create private structure */
	launch = g_slice_new0(Launch);
	launch->icon = g_strdup(json_object_get_string_member(object, "icon"));
	launch->name = g_strdup(json_object_get_string_member(object, "name"));
	launch->exec = g_strdup(json_object_get_string_member(object, "execute"));

	g_print("%s\n", launch->icon);

	widget->priv = (gpointer)launch;
	
}

JshModuleClass module_class = {
	name: "launch",
	version: "1.0",
	description: N_("Lauch Application"),

	constructor: launch_constructor,
	config: launch_config
};
