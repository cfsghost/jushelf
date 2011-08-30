#ifndef JSH_WIDGET_H
#define JSH_WIDGET_H

#include <clutter/clutter.h>
#include <jushelf/module.h>

typedef enum {
	JSH_ORIENTATION_HORIZONTAL,
	JSH_ORIENTATION_VERTICAL
} JshOrientation;

struct _JshWidget { 
	JshOrientation orientation;
	ClutterActor *container;
	JshModule *module;
	gpointer priv;
};
typedef struct _JshWidget JshWidget;

#endif
