#include <stdlib.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <sys/stat.h>
#include <clutter/clutter.h>
#include <clutter/x11/clutter-x11.h>
#include <jushelf/conf.h>
#include <jushelf/module.h>
#include "jushelf.h"
#include "shelf.h"
#include "module.h"

JshModule *
jsh_module_load(const gchar *filename)
{
	JshModule *module;

	DEBUG("Load module: %s\n", filename);

	module = (JshModule *)g_slice_new0(JshModule);

	module->gmodule = g_module_open(filename, G_MODULE_BIND_LAZY);
	if (module->gmodule == NULL) {
		g_slice_free(JshModule, module);
		DEBUG("Failed to load: %s\n", filename);
		return NULL;
	}

	if (!g_module_symbol(module->gmodule, "module_class", (gpointer *)&module->module_class) ||
		!module->module_class) {
		g_module_close(module->gmodule);
		g_slice_free(JshModule, module);
		return NULL;
	}

	return module;
}

JshModule *
jsh_module_find_from_file(const gchar *module_name)
{
	JshModule *module;
	const char *filename;
	char *fullpath;
	GDir *dir;

	DEBUG("Finding Module from path: %s\n", PACKAGE_LIB_DIR "/jushelf");

	dir = g_dir_open(PACKAGE_LIB_DIR "/jushelf", 0, NULL);
	if (dir == NULL)
		return NULL;

	/* Search */
	while((filename = g_dir_read_name(dir)) != NULL) {
		if (g_str_has_suffix(filename, ".so")) {
			/* Load Module */
			fullpath = g_build_filename(PACKAGE_LIB_DIR "/jushelf", filename, NULL);
			module = jsh_module_load(fullpath);
			if (!module) {
				g_free(fullpath);
				continue;
			}

			/* Check module name */
			if (strcmp(module->module_class->name, module_name) == 0) {
				DEBUG("Found Module \"%s\"\n", filename);
				g_dir_close(dir);
				g_free(fullpath);
				return module;
			}

			g_free(fullpath);
		}
	}

	g_dir_close(dir);

	DEBUG("No found Module!\n");

	return NULL;
}

JshModule *
jsh_module_find(JuShelf *jushelf, const gchar *module_name)
{
	JshModule *module;
	gint i;

	DEBUG("Finding Module from list\n");

	/* Find from modules list */
	for (i = 0; jushelf->modules->len; ++i) {
		module = (JshModule *)g_ptr_array_index(jushelf->modules, i);
		if (strcmp(module->module_class->name, module_name) == 0)
			return module;
	}

	/* Search external modules */
	module = jsh_module_find_from_file(module_name);	
	if (!module)
		return NULL;

	/* Add to modules list */
	g_ptr_array_add(jushelf->modules, module);

	return module;
}

void
jsh_module_constructor(JshWidget *widget)
{
	widget->module->module_class->constructor(widget);
}

void
jsh_module_config(JshWidget *widget, JsonNode *node)
{
	widget->module->module_class->config(widget, node);
}
