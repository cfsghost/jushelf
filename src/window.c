#include <stdlib.h>
#include <glib.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <sys/stat.h>
#include <clutter/clutter.h>
#include <clutter/x11/clutter-x11.h>
#include "jushelf.h"
#include "window.h"

void
jsh_window_set_type(Display *disp, Window w, JshWindowType wtype)
{
	Atom wt_atom;
	Atom type_atom;

	DEBUG("Set window type is %d\n", wtype);

	switch(wtype) {
	case JSH_WINDOW_TYPE_DESKTOP:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
		break;
	case JSH_WINDOW_TYPE_DOCK:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DOCK", False);
		break;
	case JSH_WINDOW_TYPE_TOOLBAR:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
		break;
	case JSH_WINDOW_TYPE_MENU:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_MENU", False);
		break;
	case JSH_WINDOW_TYPE_UTILITY:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_UTILITY", False);
		break;
	case JSH_WINDOW_TYPE_SPLASH:
		type_atom = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_SPLASH", False);
		break;
	case JSH_WINDOW_TYPE_DIALOG:
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

void
jsh_window_set_decorator(Display *disp, Window w, gboolean enabled)
{
	XSetWindowAttributes attr;

	DEBUG("Set window(%d) has decorator is %s\n", w, enabled ? "True" : "False");

	if (enabled) {
		/* Enable decorator */
		attr.override_redirect = False;
	} else {
		/* Disable decorator */
		attr.override_redirect = True;
	}

	XGrabServer(disp);
	XChangeWindowAttributes(disp, w, CWOverrideRedirect, &attr);
	XUngrabServer(disp);
}
