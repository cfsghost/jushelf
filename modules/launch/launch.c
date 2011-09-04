#include <glib.h>
#include <glib/gi18n.h>
#include <clutter/clutter.h>
#include <jushelf/module.h>
#include <jushelf/conf.h>
#include <jushelf/widget.h>

typedef struct {
	ClutterActor *container;
	ClutterActor *icon_actor;
	ClutterActor *label_actor;
	ClutterActor *label_text_actor;
	ClutterActor *label_bg_actor;
	ClutterColor *bg_color;
	ClutterColor *text_color;
	ClutterState *icon_state;
	ClutterState *label_state;
	gchar *icon;
	gchar *name;
	gchar *exec;
} Launch;

static gboolean
launch_press_cb(JshWidget *widget)
{
	Launch *launch = (Launch *)widget->priv;

	clutter_state_set_state(launch->icon_state, "press");
	return TRUE;
}

static gboolean
launch_release_cb(JshWidget *widget)
{
	Launch *launch = (Launch *)widget->priv;

	if (launch->exec)
		g_spawn_command_line_async(launch->exec, NULL);

	clutter_state_set_state(launch->icon_state, "release");
	return TRUE;
}

static gboolean
launch_leave_cb(JshWidget *widget)
{
	Launch *launch = (Launch *)widget->priv;

	clutter_state_set_state(launch->icon_state, "normal");
	clutter_state_set_state(launch->label_state, "deactivate");
	return TRUE;
}

static gboolean
launch_enter_cb(JshWidget *widget)
{
	Launch *launch = (Launch *)widget->priv;

	/* Raise top */
	clutter_actor_raise_top(widget->container);

	clutter_state_set_state(launch->icon_state, "active");
	clutter_state_set_state(launch->label_state, "active");
		
	return TRUE;
}

static ClutterActor *
launch_round_rectangle_new(gint width, gint height, ClutterColor *color)
{
	ClutterActor *actor;
	cairo_t *cr;
	gdouble radius = 8.0;
	gdouble degrees = G_PI / 180.0;

	/* Initializing a new actor to use Cairo */
	actor = clutter_cairo_texture_new(width, height);
	cr = clutter_cairo_texture_create(CLUTTER_CAIRO_TEXTURE(actor));

	/* Clear */
	cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint(cr);
	cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

	/* path for rounded rectangle */
	cairo_new_sub_path(cr);
	cairo_arc(cr, width - radius, radius, radius, -90 * degrees, 0);
	cairo_arc(cr, width - radius, height - radius, radius, 0, 90 * degrees);
	cairo_arc(cr, radius, height - radius, radius, 90 * degrees, 180 * degrees);
	cairo_arc(cr, radius, radius, radius, 180 * degrees, 270 * degrees);
	cairo_close_path(cr);

	cairo_set_source_rgb(cr, (gdouble)color->red / 255, (gdouble)color->green / 255, (gdouble)color->blue / 255);
	cairo_fill(cr);

	cairo_destroy(cr);

	return actor;
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
		launch->text_color = clutter_color_new(0xff, 0xff, 0xff, 0xff);
	} else {
		launch->name = g_strdup("Application");
		launch->text_color = clutter_color_new(0xff, 0xff, 0xff, 0xff);
	}

	launch->bg_color = clutter_color_new(0x33, 0x33, 0x33, 0xff);

	widget->priv = (gpointer)launch;

	/* Initializing object */
	launch->container = clutter_group_new();
	launch->icon_actor = clutter_texture_new_from_file(launch->icon, NULL);
	clutter_actor_set_reactive(launch->icon_actor, TRUE);
	clutter_texture_set_keep_aspect_ratio(CLUTTER_TEXTURE(launch->icon_actor), TRUE);
	clutter_texture_set_filter_quality(CLUTTER_TEXTURE(launch->icon_actor), CLUTTER_TEXTURE_QUALITY_HIGH);
	clutter_container_add_actor(CLUTTER_CONTAINER(launch->container), launch->icon_actor);
	clutter_container_add_actor(CLUTTER_CONTAINER(widget->container), launch->container);

	/* Label */
	launch->label_actor = clutter_group_new();
	clutter_actor_set_opacity(launch->label_actor, 0x00);
	clutter_actor_set_anchor_point_from_gravity(launch->label_actor, CLUTTER_GRAVITY_CENTER);
	clutter_container_add_actor(CLUTTER_CONTAINER(launch->container), launch->label_actor);

	/* Label text */
	launch->label_text_actor = clutter_text_new_full("Sans bold 9", launch->name, launch->text_color);
	clutter_container_add_actor(CLUTTER_CONTAINER(launch->label_actor), launch->label_text_actor);

	/* Label background */
	launch->label_bg_actor = launch_round_rectangle_new((gint)(clutter_actor_get_width(launch->label_actor) + 20), (gint)(clutter_actor_get_height(launch->label_actor) + 8), launch->bg_color);
	clutter_container_add_actor(CLUTTER_CONTAINER(launch->label_actor), launch->label_bg_actor);
	clutter_actor_lower(launch->label_bg_actor, launch->label_text_actor);

	/* Put Text at center */
	clutter_actor_set_position(launch->label_text_actor,
		(clutter_actor_get_width(launch->label_bg_actor) - clutter_actor_get_width(launch->label_text_actor)) * 0.5,
		(clutter_actor_get_height(launch->label_bg_actor) - clutter_actor_get_height(launch->label_text_actor)) * 0.5);

	/* Set icon behavior */
	launch->icon_state = clutter_state_new();
	clutter_state_set_duration(launch->icon_state, NULL, NULL, 360);
	g_object_set(G_OBJECT(launch->icon_actor),
		"scale-gravity", CLUTTER_GRAVITY_CENTER,
		"rotation-center-z-gravity", CLUTTER_GRAVITY_CENTER,
		NULL);
	clutter_state_set(launch->icon_state, NULL, "active",
		launch->icon_actor, "scale-x", CLUTTER_EASE_OUT_CUBIC, 1.5,
		launch->icon_actor, "scale-y", CLUTTER_EASE_OUT_CUBIC, 1.5,
		launch->icon_actor, "rotation-angle-z", CLUTTER_EASE_OUT_CUBIC, 12.0,
		NULL);
	clutter_state_set(launch->icon_state, NULL, "normal",
		launch->icon_actor, "opacity", CLUTTER_EASE_OUT_CUBIC, 0xff,
		launch->icon_actor, "scale-x", CLUTTER_EASE_OUT_CUBIC, 1.0,
		launch->icon_actor, "scale-y", CLUTTER_EASE_OUT_CUBIC, 1.0,
		launch->icon_actor, "rotation-angle-z", CLUTTER_EASE_OUT_CUBIC, .0,
		NULL);
	clutter_state_set(launch->icon_state, NULL, "press",
		launch->icon_actor, "opacity", CLUTTER_EASE_OUT_CUBIC, 0x88,
		NULL);
	clutter_state_set(launch->icon_state, NULL, "release",
		launch->icon_actor, "opacity", CLUTTER_EASE_OUT_CUBIC, 0xff,
		NULL);

	/* Set label behavior */
	launch->label_state = clutter_state_new();
	clutter_state_set_duration(launch->label_state, NULL, NULL, 360);
	clutter_state_set(launch->label_state, NULL, "active",
		launch->label_actor, "opacity", CLUTTER_EASE_OUT_CUBIC, 0xff,
		NULL);
	clutter_state_set(launch->label_state, NULL, "deactivate",
		launch->label_actor, "opacity", CLUTTER_EASE_OUT_CUBIC, 0x00,
		NULL);

	/* Define events */
	g_signal_connect_swapped(launch->icon_actor, "button-press-event",
		G_CALLBACK(launch_press_cb), widget);
	g_signal_connect_swapped(launch->icon_actor, "button-release-event",
		G_CALLBACK(launch_release_cb), widget);
	g_signal_connect_swapped(launch->icon_actor, "enter-event",
		G_CALLBACK(launch_enter_cb), widget);
	g_signal_connect_swapped(launch->icon_actor, "leave-event",
		G_CALLBACK(launch_leave_cb), widget);
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

		/* Top */
		if (widget->place >= JSH_PLACE_TOP && widget->place <= JSH_PLACE_TOP_RIGHT) {
			/* Label position */
			clutter_actor_set_position(launch->label_actor,
				clutter_actor_get_width(launch->icon_actor) * 0.5,
				clutter_actor_get_height(launch->icon_actor) * 1.5);

		/* Bottom */
		} else if (widget->place >= JSH_PLACE_BOTTOM && widget->place <= JSH_PLACE_BOTTOM_RIGHT) {
			/* Label position */
			clutter_actor_set_position(launch->label_actor,
				clutter_actor_get_width(launch->icon_actor) * 0.5,
				-clutter_actor_get_height(launch->icon_actor) * 0.5);
		}

	} else {
		clutter_actor_set_request_mode(launch->container, CLUTTER_REQUEST_HEIGHT_FOR_WIDTH);
		clutter_actor_set_width(launch->container, clutter_actor_get_width(widget->container));
		clutter_actor_set_request_mode(launch->icon_actor, CLUTTER_REQUEST_HEIGHT_FOR_WIDTH);
		clutter_actor_set_width(launch->icon_actor, clutter_actor_get_width(widget->container));
	}

	clutter_actor_get_size(launch->icon_actor, &widget->width, &widget->height);
}

JshModuleClass module_class = {
	name: "launch",
	version: "1.0",
	description: N_("Lauch Application"),

	constructor: launch_constructor,
	resize: launch_resize
};
