#include <stdlib.h>
#include <glib.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <sys/stat.h>
#include <clutter/clutter.h>
#include <clutter/x11/clutter-x11.h>
#include "jushelf.h"
#include "shelf.h"
#include "window.h"

static gboolean
jsh_shelf_event_handler(ClutterActor *actor, ClutterEvent *event, gpointer data)
{
	JshShelf *shelf = (JshShelf *)data;

	if (event->type == CLUTTER_ENTER) {
		if (event->any.source == shelf->window) {
			jsh_shelf_reset_place(shelf, shelf->size * 1.8);
			clutter_state_set_state(shelf->state, "active");
			return TRUE;
		}

	} else if (event->type == CLUTTER_LEAVE) {
		if (event->any.source == shelf->window && event->crossing.related == NULL) {
			clutter_state_set_state(shelf->state, "deactivate");

			return TRUE;
		}
	}

	return FALSE;
}

static void
jsh_shelf_state_completed(ClutterState *state, gpointer data)
{
	JshShelf *shelf = (JshShelf *)data;

	if (g_str_equal(clutter_state_get_state(shelf->state), "deactivate")) {
		/* Hide window after effect */
		jsh_shelf_reset_place(shelf, 0);
	}
}

JshShelf *
jsh_shelf_new(JuShelf *jushelf)
{
	JshShelf *shelf;

	shelf = (JshShelf *)g_slice_new0(JshShelf);
	shelf->parent = jushelf;
	shelf->widgets = g_ptr_array_new();

	return shelf;
}

void
jsh_shelf_init(JshShelf *shelf)
{
	Window w;
	Display *disp;
	gint i;
	int screen;
	gfloat offset = 0;
	JshWidget *widget;

	DEBUG("Initializing shelf: \"%s\"\n", shelf->name);
	DEBUG("Create Window of Shelf\n");

	/* Initializing stage of shelf */
	shelf->window = clutter_stage_new();
	shelf->container = clutter_group_new();
	clutter_stage_set_title(CLUTTER_STAGE(shelf->window), shelf->name);
	clutter_stage_set_use_alpha(CLUTTER_STAGE(shelf->window), TRUE);
	clutter_actor_set_opacity(shelf->window, shelf->opacity);
	clutter_container_add_actor(CLUTTER_CONTAINER(shelf->window), shelf->container);

	/* Autohide */
	if (shelf->autohide) {
		DEBUG("Enable Autohide\n");
		g_signal_connect(shelf->window, "captured-event",
			G_CALLBACK(jsh_shelf_event_handler), shelf);
	}

	/* State of shelf */
	shelf->state = clutter_state_new();
	clutter_state_set_duration(shelf->state, NULL, NULL, 720);
	if (shelf->orientation == JSH_ORIENTATION_HORIZONTAL) {
		/* Top */
		if (shelf->place >= JSH_PLACE_TOP && shelf->place <= JSH_PLACE_TOP_RIGHT) {
			clutter_state_set(shelf->state, NULL, "active",
				shelf->container, "y", CLUTTER_EASE_OUT_QUINT, shelf->size * 0.5,
				shelf->container, "opacity", CLUTTER_EASE_OUT_QUINT, 0xff,
				NULL);
			clutter_state_set(shelf->state, NULL, "deactivate",
				shelf->container, "y", CLUTTER_EASE_OUT_QUAD, (gdouble)-shelf->size,
				shelf->container, "opacity", CLUTTER_EASE_OUT_QUAD, 0x00,
				NULL);
			clutter_state_set_duration(shelf->state, "active", "deactivate", 180);

		/* Bottom */
		} else if (shelf->place >= JSH_PLACE_BOTTOM && shelf->place <= JSH_PLACE_BOTTOM_RIGHT) {
			clutter_state_set(shelf->state, NULL, "active",
				shelf->container, "y", CLUTTER_EASE_OUT_QUINT, (gdouble)shelf->size * 0.7,
				shelf->container, "opacity", CLUTTER_EASE_OUT_QUINT, 0xff,
				NULL);
			clutter_state_set(shelf->state, NULL, "deactivate",
				shelf->container, "y", CLUTTER_EASE_OUT_QUAD, (gdouble)shelf->size,
				shelf->container, "opacity", CLUTTER_EASE_OUT_QUAD, 0x00,
				NULL);
			clutter_state_set_duration(shelf->state, "active", "deactivate", 180);
		}
	}
	g_signal_connect(shelf->state, "completed", G_CALLBACK(jsh_shelf_state_completed), shelf);

	/* shelf size */
	clutter_actor_set_size(shelf->window, shelf->size * 2, shelf->size * 2);

	/* Initializing widget */
	offset = shelf->size;
	for (i = 0; i < shelf->widgets->len; ++i) {
		widget = g_ptr_array_index(shelf->widgets, i);

		if (shelf->orientation == JSH_ORIENTATION_HORIZONTAL) {
			/* set widget size */
			clutter_actor_set_request_mode(widget->container, CLUTTER_REQUEST_WIDTH_FOR_HEIGHT);
			clutter_actor_set_height(widget->container, shelf->size * 0.75);

			/* notify widget to resize */
			jsh_widget_resize(shelf->parent, widget);

			/* widget position */
			clutter_actor_set_position(widget->container, (gfloat)offset, 0.0);
			clutter_container_add_actor(CLUTTER_CONTAINER(shelf->container), widget->container);

			/* Update window size */
			offset += widget->width;
			clutter_actor_set_width(shelf->window, offset + shelf->size);
		} else {
			/* set widget size */
			clutter_actor_set_request_mode(widget->container, CLUTTER_REQUEST_HEIGHT_FOR_WIDTH);
			clutter_actor_set_width(widget->container, shelf->size * 0.75);

			/* notify widget to resize */
			jsh_widget_resize(shelf->parent, widget);

			/* put widget on center */
			offset += widget->height;
			clutter_actor_set_position(widget->container, (gfloat)shelf->size, (gfloat)offset);
			clutter_container_add_actor(CLUTTER_CONTAINER(shelf->container), widget->container);

			/* Update window size */
			clutter_actor_set_height(shelf->window, offset + widget->height);
		}
	}

	clutter_actor_show(shelf->window);

	DEBUG("Initializing Window of Shelf\n");
	jsh_shelf_window_init(shelf);

	/* Place by default */
	clutter_state_set_state(shelf->state, "deactivate");
	jsh_shelf_update_place(shelf);
}

void
jsh_shelf_update_place(JshShelf *shelf)
{
	if (shelf->autohide) {
		jsh_shelf_reset_place(shelf, 0);
	} else {
		jsh_shelf_reset_place(shelf, shelf->size * 1.8);
		clutter_state_set_state(shelf->state, "active");
	}
}

void
jsh_shelf_reset_place(JshShelf *shelf, gfloat offset)
{
	Window w;
	Display *disp;
	int screen;

	/* Get X11 window of stage */
	w = clutter_x11_get_stage_window(CLUTTER_STAGE(shelf->window));
	disp = clutter_x11_get_default_display();
	screen = clutter_x11_get_default_screen();

	switch(shelf->place) {
	case JSH_PLACE_TOP:
		XMoveWindow(disp, w,
			((gfloat)shelf->parent->screen_width - clutter_actor_get_width(shelf->window)) * 0.5,
			- clutter_actor_get_height(shelf->window) + 2 + offset);
		break;
	case JSH_PLACE_BOTTOM:
		XMoveWindow(disp, w,
			((gfloat)shelf->parent->screen_width - clutter_actor_get_width(shelf->window)) * 0.5,
			(gfloat)shelf->parent->screen_height - 2 - offset);
		break;
	}
}

void
jsh_shelf_window_init(JshShelf *shelf)
{
	Window w;
	Display *disp;
	int screen;

	/* Get X11 window of stage */
	w = clutter_x11_get_stage_window(CLUTTER_STAGE(shelf->window));
	disp = clutter_x11_get_default_display();
	screen = clutter_x11_get_default_screen();

	/* Set window style */
	jsh_window_set_type(disp, w, JSH_WINDOW_TYPE_DOCK);
	jsh_window_set_decorator(disp, w, FALSE);
}

