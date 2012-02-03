#ifndef JUSHELF_H
#define JUSHELF_H

#ifdef _DEBUG
#define DEBUG(format, args...) printf("[%s:%d] "format, __FILE__, __LINE__, ##args)
#else
#define DEBUG(args...)
#endif

#include <jushelf/conf.h>
#include <jushelf/widget.h>
#include <jushelf/module.h>

typedef enum {
	JSH_WINDOW_TYPE_NORMAL,
	JSH_WINDOW_TYPE_DESKTOP,
	JSH_WINDOW_TYPE_DOCK,
	JSH_WINDOW_TYPE_TOOLBAR,
	JSH_WINDOW_TYPE_MENU,
	JSH_WINDOW_TYPE_UTILITY,
	JSH_WINDOW_TYPE_SPLASH,
	JSH_WINDOW_TYPE_DIALOG
} JshWindowType;

typedef struct _JuShelf JuShelf;

typedef struct {
	JuShelf *parent;
	ClutterActor *window;
	ClutterActor *container;
	ClutterState *state;

	/* Properties */
	gchar *name;
	gint size;
	guint8 opacity;
	gboolean autohide;
	JshPlace place;
	JshOrientation orientation;
	GPtrArray *widgets;
} JshShelf;

struct _JuShelf {
	gint screen_width;
	gint screen_height;

	/* XRandr Extension */
	gint rr_major_version;
	gint rr_minor_version;
	gint xrandr_event_base;
	gint xrandr_error_base;

	GPtrArray *modules;
	GPtrArray *shelves;
};

#endif
