#ifndef JSH_MODULE_CLASS_H
#define JSH_MODULE_CLASS_H

#include <glib.h>
#include <gmodule.h>
#include <jushelf/conf.h>

typedef struct _JshWidget JshWidget;
struct _JshModuleClass {
	gchar *name;
	gchar *version;
	gchar *description;
	void (*constructor)(JshWidget *, JsonNode *);
	void (*resize)(JshWidget *);
};
typedef struct _JshModuleClass JshModuleClass;

struct _JshModule {
	GModule *gmodule;
	JshModuleClass *module_class;
};
typedef struct _JshModule JshModule;

#endif
