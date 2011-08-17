#include <stdlib.h>
#include <glib.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <clutter/clutter.h>
#include <clutter/x11/clutter-x11.h>
#include "jushelf.h"

void ju_set_window_type(Display *disp, Window w, JuWindowType wtype)
{
	Atom wt_atom;
	Atom type_atom;

	switch(wtype) {
	case JU_WINDOW_TYPE_DESKTOP:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
		break;
	case JU_WINDOW_TYPE_DOCK:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DOCK", False);
		break;
	case JU_WINDOW_TYPE_TOOLBAR:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
		break;
	case JU_WINDOW_TYPE_MENU:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_MENU", False);
		break;
	case JU_WINDOW_TYPE_UTILITY:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_UTILITY", False);
		break;
	case JU_WINDOW_TYPE_SPLASH:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_SPLASH", False);
		break;
	case JU_WINDOW_TYPE_DIALOG:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DIALOG", False);
		break;
	default:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_NORMAL", False);
	}

	wt_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE", False);

	XChangeProperty(disp, w,
		wt_atom,
		XA_ATOM, 32, PropModeReplace,
		(unsigned char *)&type_atom, 1);
}

int
main(int argc, char *argv[])
{
	XSetWindowAttributes attr;
	ClutterActor *stage;
	Window stage_win;
	Display *disp;

	clutter_x11_set_use_argb_visual(TRUE);

	clutter_init(&argc, &argv);

	/* Create a new stage */
	stage = clutter_stage_new();
	clutter_stage_set_title(CLUTTER_STAGE(stage), "JuShelf");
	clutter_stage_set_user_resizable(CLUTTER_STAGE(stage), FALSE);
	clutter_stage_set_use_alpha(CLUTTER_STAGE(stage), TRUE);
	clutter_actor_set_opacity(stage, 0x22);
	clutter_actor_set_size(stage, 600, 64);

	/* Quit when destory */
	g_signal_connect(stage, "destroy", G_CALLBACK(clutter_main_quit), NULL);

	/* Show All */
	clutter_actor_show_all(stage);

	stage_win = clutter_x11_get_stage_window(CLUTTER_STAGE(stage));
	disp = clutter_x11_get_default_display();

	/* Disable decorator */
	attr.override_redirect = True;
	XChangeWindowAttributes(disp, stage_win, CWOverrideRedirect, &attr);

	/* Dock window */
	ju_set_window_type(disp, stage_win, JU_WINDOW_TYPE_DOCK);

	/* Loop */
	clutter_main();

	return EXIT_SUCCESS;
}
