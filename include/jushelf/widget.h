#ifndef JSH_WIDGET_H
#define JSH_WIDGET_H

#include <clutter/clutter.h>
#include <jushelf/module.h>

struct _JshWidget { 
	ClutterActor *container;
	JshModule *module;
	gpointer priv;
};
typedef struct _JshWidget JshWidget;

#endif
