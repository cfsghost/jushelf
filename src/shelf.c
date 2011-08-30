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
	gfloat offset = 0;
	JshWidget *widget;

	DEBUG("Initializing shelf: \"%s\"\n", shelf->name);

	/* Initializing stage of shelf */
	shelf->window = clutter_stage_new();
	shelf->container = clutter_group_new();
	clutter_stage_set_title(CLUTTER_STAGE(shelf->window), shelf->name);
	clutter_stage_set_user_resizable(CLUTTER_STAGE(shelf->window), FALSE);
	clutter_stage_set_use_alpha(CLUTTER_STAGE(shelf->window), TRUE);
	clutter_actor_set_opacity(shelf->window, shelf->opacity);
	clutter_container_add_actor(CLUTTER_CONTAINER(shelf->window), shelf->container);

	/* shelf size */
	if (shelf->place < JSH_PLACE_LEFT)
		clutter_actor_set_height(shelf->window, shelf->size);
	else
		clutter_actor_set_width(shelf->window, shelf->size);

	clutter_actor_show(shelf->window);

	DEBUG("Create Window of Shelf\n");

	/* Get X11 window of stage */
	w = clutter_x11_get_stage_window(CLUTTER_STAGE(shelf->window));
	disp = clutter_x11_get_default_display();

	DEBUG("Initializing Window of Shelf\n");

	/* Set window style */
	jsh_window_set_type(disp, w, JSH_WINDOW_TYPE_DOCK);
	jsh_window_set_decorator(disp, w, FALSE);

	/* Initializing widget */
	for (i = 0; i < shelf->widgets->len; ++i) {
		widget = g_ptr_array_index(shelf->widgets, i);

		if (shelf->orientation == JSH_ORIENTATION_HORIZONTAL) {
			clutter_actor_set_request_mode(widget->container, CLUTTER_REQUEST_WIDTH_FOR_HEIGHT);
			clutter_actor_set_height(widget->container, shelf->size * 0.75);
			offset += clutter_actor_get_width(widget->container) * 0.5;

			/* put it on center */
			clutter_actor_set_position(widget->container, (gfloat)offset, (gfloat)shelf->size * 0.5);
		} else {
			clutter_actor_set_request_mode(widget->container, CLUTTER_REQUEST_HEIGHT_FOR_WIDTH);
			clutter_actor_set_width(widget->container, shelf->size * 0.75);
			offset += clutter_actor_get_height(widget->container) * 0.5;

			/* put it on center */
			clutter_actor_set_position(widget->container, (gfloat)shelf->size * 0.5, (gfloat)offset);
		}
		clutter_container_add_actor(CLUTTER_CONTAINER(shelf->container), widget->container);

		jsh_widget_resize(shelf->parent, widget);
	}
}
