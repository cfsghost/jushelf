#ifndef JSH_WIDGET_H
#define JSH_WIDGET_H

#include <clutter/clutter.h>
#include <jushelf/module.h>

typedef enum {
	JSH_ORIENTATION_HORIZONTAL,
	JSH_ORIENTATION_VERTICAL
} JshOrientation;

struct _JshWidget { 
	gpointer parent;
	JshOrientation orientation;
	ClutterActor *container;
	JshModule *module;
	gpointer priv;
	gfloat width;
	gfloat height;
};
typedef struct _JshWidget JshWidget;

#endif
