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

typedef enum {
	JSH_PLACE_TOP,
	JSH_PLACE_TOP_LEFT,
	JSH_PLACE_TOP_RIGHT,

	JSH_PLACE_BOTTOM,
	JSH_PLACE_BOTTOM_LEFT,
	JSH_PLACE_BOTTOM_RIGHT,

	JSH_PLACE_LEFT,
	JSH_PLACE_RIGHT
} JshPlace;

typedef struct _JuShelf JuShelf;

typedef struct {
	JuShelf *parent;
	ClutterActor *window;
	ClutterActor *container;
	GPtrArray *widgets;
	gchar *name;
	gint size;
	guint8 opacity;
	JshPlace place;
	JshOrientation orientation;
} JshShelf;

struct _JuShelf {
	GPtrArray *modules;
	GPtrArray *shelves;
};

#endif
