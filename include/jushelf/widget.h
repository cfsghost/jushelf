#ifndef JSH_WIDGET_H
#define JSH_WIDGET_H

#include <clutter/clutter.h>
#include <jushelf/module.h>

typedef enum {
	JSH_ORIENTATION_HORIZONTAL,
	JSH_ORIENTATION_VERTICAL
} JshOrientation;

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


struct _JshWidget { 
	gpointer parent;
	JshPlace place;
	JshOrientation orientation;
	ClutterActor *container;
	JshModule *module;
	gpointer priv;
	gfloat width;
	gfloat height;
};
typedef struct _JshWidget JshWidget;

#endif
