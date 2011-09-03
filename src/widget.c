#include <stdlib.h>
#include <glib.h>
#include <json-glib/json-glib.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <sys/stat.h>
#include <clutter/clutter.h>
#include <clutter/x11/clutter-x11.h>
#include <jushelf/conf.h>
#include <jushelf/widget.h>
#include "jushelf.h"
#include "shelf.h"
#include "widget.h"
#include "module.h"

JshWidget *
jsh_widget_new(JuShelf *jushelf, const gchar *module_name)
{
	JshWidget *widget;

	DEBUG("Create a widget - \"%s\"\n", module_name);
	widget = (JshWidget *)g_slice_new0(JshWidget);
	widget->module = jsh_module_find(jushelf, module_name);

	return widget;
}

void
jsh_widget_init(JuShelf *jushelf, JshShelf *shelf, JshWidget *widget, JsonNode *node)
{
	widget->parent = (gpointer)shelf;
	widget->container = clutter_group_new();

	jsh_module_constructor(widget, node);
}

void
jsh_widget_resize(JuShelf *jushelf, JshWidget *widget)
{
	DEBUG("Resize Widget\n");
	jsh_module_resize(widget);
}
