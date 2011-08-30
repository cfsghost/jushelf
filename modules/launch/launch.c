#include <glib.h>
#include <glib/gi18n.h>
#include <clutter/clutter.h>
#include <jushelf/module.h>
#include <jushelf/conf.h>
#include <jushelf/widget.h>

typedef struct {
	ClutterActor *container;
	ClutterActor *icon_actor;
	ClutterState *state;
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

static gboolean
launch_leave_cb(Launch *launch)
{
	g_print("leave\n");
	clutter_state_set_state(launch->state, "normal");
	return TRUE;
}

static gboolean
launch_enter_cb(ClutterActor *actor, ClutterEvent *event, gpointer data)
{
	Launch *launch = (Launch *)data;

	g_print("enter\n");
	clutter_state_set_state(launch->state, "active");
	return FALSE;
}

static void
launch_constructor(JshWidget *widget, JsonNode *node)
{
	Launch *launch;
	JsonObject *object;

	/* Create private structure */
	launch = g_slice_new0(Launch);

	if (node) {
		/* Read Settings from config file */
		object = json_node_get_object(node);
		launch->icon = g_strdup(json_object_get_string_member(object, "icon"));
		launch->name = g_strdup(json_object_get_string_member(object, "name"));
		launch->exec = g_strdup(json_object_get_string_member(object, "execute"));
	} else {
		launch->name = g_strdup("Application");
	}

	widget->priv = (gpointer)launch;

	/* Initializing object */
	launch->container = clutter_group_new();
	launch->icon_actor = clutter_texture_new_from_file(launch->icon, NULL);
	clutter_texture_set_keep_aspect_ratio(CLUTTER_TEXTURE(launch->icon_actor), TRUE);
	clutter_container_add_actor(CLUTTER_CONTAINER(launch->container), launch->icon_actor);
	clutter_container_add_actor(CLUTTER_CONTAINER(widget->container), launch->container);

	/* Set behavior */
	clutter_actor_set_reactive(launch->container, TRUE);

	launch->state = clutter_state_new();
	g_object_set(G_OBJECT(launch->container),
		"scale-gravity", CLUTTER_GRAVITY_CENTER,
		NULL);
	clutter_state_set(launch->state, NULL, "active",
		launch->container, "scale-x", CLUTTER_EASE_OUT_CUBIC, 1.5,
		launch->container, "scale-y", CLUTTER_EASE_OUT_CUBIC, 1.5,
		NULL);
	clutter_state_set(launch->state, NULL, "normal",
		launch->container, "scale-x", CLUTTER_EASE_OUT_CUBIC, 1.0,
		launch->container, "scale-y", CLUTTER_EASE_OUT_CUBIC, 1.0,
		NULL);
	clutter_state_set_duration(launch->state, NULL, NULL, 360);

	/* Define events */
	g_signal_connect_swapped(launch->container, "button-press-event",
		G_CALLBACK(launch_clicked_cb), launch);
	g_signal_connect(launch->container, "enter-event",
		G_CALLBACK(launch_enter_cb), launch);
	g_signal_connect_swapped(launch->container, "leave-event",
		G_CALLBACK(launch_leave_cb), launch);

	g_print("Constructor\n");
}

static void
launch_resize(JshWidget *widget)
{
	Launch *launch;

	launch = (Launch *)widget->priv;

	if (widget->orientation == JSH_ORIENTATION_HORIZONTAL) {
		clutter_actor_set_request_mode(launch->container, CLUTTER_REQUEST_WIDTH_FOR_HEIGHT);
		clutter_actor_set_height(launch->container, clutter_actor_get_height(widget->container));
		clutter_actor_set_request_mode(launch->icon_actor, CLUTTER_REQUEST_WIDTH_FOR_HEIGHT);
		clutter_actor_set_height(launch->icon_actor, clutter_actor_get_height(widget->container));
	} else {
		clutter_actor_set_request_mode(launch->container, CLUTTER_REQUEST_HEIGHT_FOR_WIDTH);
		clutter_actor_set_width(launch->container, clutter_actor_get_width(widget->container));
		clutter_actor_set_request_mode(launch->icon_actor, CLUTTER_REQUEST_HEIGHT_FOR_WIDTH);
		clutter_actor_set_width(launch->icon_actor, clutter_actor_get_width(widget->container));
	}
}

JshModuleClass module_class = {
	name: "launch",
	version: "1.0",
	description: N_("Lauch Application"),

	constructor: launch_constructor,
	resize: launch_resize
};
