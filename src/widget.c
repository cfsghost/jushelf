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
jsh_widget_init(JuShelf *jushelf, JshWidget *widget)
{
	widget->container = clutter_group_new();
	clutter_actor_set_anchor_point_from_gravity(widget->container, CLUTTER_GRAVITY_CENTER);

	jsh_module_constructor(widget);
}

void
jsh_widget_config(JuShelf *jushelf, JshWidget *widget, JsonNode *node)
{
	DEBUG("Loading Widget Settings\n");
	jsh_module_config(widget, node);
}
