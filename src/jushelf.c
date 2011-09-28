#include <stdlib.h>
#include <glib.h>
#include <gmodule.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <sys/stat.h>
#include <clutter/clutter.h>
#include <clutter/x11/clutter-x11.h>
#include <jushelf/conf.h>
#include <jushelf/widget.h>
#include "jushelf.h"
#include "conf.h"
#include "shelf.h"
#include "widget.h"

JshShelf *
jsh_build_shelf(JuShelf *jushelf, JshConf *conf, JsonNode *node, const gchar *shelf_name)
{
	JshShelf *shelf;
	JshWidget *widget;
	JsonNode *group_node;
	JsonNode *widget_node;
	JsonObject *object;
	GList *widgets = NULL;
	GList *listnode = NULL;
	const gchar *module_name;

	DEBUG("Create a shelf - \"%s\"\n", shelf_name);
	object = json_node_get_object(node);

	/* Initializing shelf */
	shelf = jsh_shelf_new(jushelf);	
	shelf->name = g_strdup(shelf_name);
	shelf->size = json_object_get_int_member(object, "size");
	shelf->opacity = json_object_get_int_member(object, "opacity");
	shelf->autohide = json_object_get_int_member(object, "autohide");
	shelf->place = json_object_get_int_member(object, "place");
	if (shelf->place < JSH_PLACE_LEFT)
		shelf->orientation = JSH_ORIENTATION_HORIZONTAL;
	else
		shelf->orientation = JSH_ORIENTATION_VERTICAL;

	DEBUG("place=%d\n", shelf->place);

	/*  Initializing widgets */
	group_node = jsh_conf_get_member(conf, node, "widgets", NULL);

	widgets = json_array_get_elements(json_node_get_array(group_node));
	for (listnode = widgets; listnode; listnode = g_list_next(listnode)) {
		widget_node = (JsonNode *)listnode->data;

		/* Get module name */
		object = json_node_get_object(widget_node);
		module_name = json_object_get_string_member(object, "module");
		
		/* create widgets */
		widget = jsh_widget_new(jushelf, module_name);
		widget->place = shelf->place;
		widget->orientation = shelf->orientation;
		g_ptr_array_add(shelf->widgets, widget);

		/* call widget function to read config */
		jsh_widget_init(jushelf, shelf, widget, widget_node);
	}

	return shelf;
}

JshShelf *
jsh_build_default_shelf(JuShelf *jushelf)
{
	JshShelf *shelf;

	DEBUG("Create a default shelf\n");
	shelf = jsh_shelf_new(jushelf);	
	shelf->name = g_strdup("default");
	shelf->place = JSH_PLACE_BOTTOM;

	/* TODO: Initializing widgets */

	return shelf;
}

void
jsh_load_main_config(JuShelf *jushelf, gchar *path)
{
	JshShelf *shelf;
	JshConf *conf;
	JsonNode *group_node;
	GList *members = NULL;
	GList *listnode = NULL;
	gchar *member;

	DEBUG("Load Settings\n");
	conf = jsh_conf_new();

	/* Load config file */
	if (jsh_conf_load_from_file(conf, path)) {
		DEBUG("Read general settings\n");
		group_node = jsh_conf_get_member(conf, NULL, "general", NULL);
		if (!group_node) {
			DEBUG("No general setting was defined in config\n");
			/* TODO: Build up default general settings here */
		} else {
			/* TODO: Load general settings here */
		}

		DEBUG("Read all settings of shelves\n");
		group_node = jsh_conf_get_member(conf, NULL, "shelves", NULL);
		if (!group_node) {
			DEBUG("No shelf was defined in config\n");
//			shelf = jsh_build_default_shelf(jushelf);
//			g_ptr_array_add(jushelf->shelves, shelf);
		} else {
			/* Read all shelves */
			DEBUG("Found shelf was defined\n");

			/* Initializing all shelves */
			members = jsh_conf_get_members(conf, group_node, NULL);
			for (listnode = members; listnode; listnode = g_list_next(listnode)) {
				member = (gchar *)listnode->data;

				/* Build up self */
				shelf = jsh_build_shelf(jushelf, conf, jsh_conf_get_member(conf, group_node, member, NULL), member);
				g_ptr_array_add(jushelf->shelves, shelf);
			}
		}
	} else {
		/* TODO: Build up default general settings here */
	}

	/* Build up a default shelf if no shelf doesn't exist */
	if (jushelf->shelves->len == 0) {
		shelf = jsh_build_default_shelf(jushelf);
		g_ptr_array_add(jushelf->shelves, shelf);
	}

	/* TODO: Release Conf object */
}

void
jsh_load_default_settings(JuShelf *jushelf)
{
	JshShelf *shelf;

	/* Create a default shelf */
	shelf = jsh_build_default_shelf(jushelf);
	g_ptr_array_add(jushelf->shelves, shelf);
}

void
jsh_config_file_init(JuShelf *jushelf)
{
	gchar *confdir;
	gchar *confpath;

	/* Main config path */
	confdir = g_build_filename(g_get_user_config_dir(), "jushelf" , NULL);
	DEBUG("Check config path: %s\n", confdir);
	if (!g_file_test(confdir, G_FILE_TEST_EXISTS))
		g_mkdir_with_parents(confdir, S_IRUSR | S_IWUSR | S_IXUSR);

	/* Main config */
	confpath = g_build_filename(confdir, "settings.conf", NULL);
	DEBUG("Check config file: %s\n", confpath);
	if (!g_file_test(confpath, G_FILE_TEST_EXISTS)) {
		jsh_load_default_settings(jushelf);
	}

	jsh_load_main_config(jushelf, confpath);
}

static gboolean
jsh_event_poll(JuShelf *jushelf)
{
	Display *disp;
	int screen;
	XWindowAttributes windowattr;
	gint i;

	/* Get current display and screen */
	disp = clutter_x11_get_default_display();
	screen = clutter_x11_get_default_screen();
	XGetWindowAttributes(disp, RootWindow(disp, screen), &windowattr);

	if (jushelf->screen_width != windowattr.width ||
		jushelf->screen_height != windowattr.height) {
		DEBUG("Resolution has been changed, update shelf position right now!\n");
		jushelf->screen_width = windowattr.width;
		jushelf->screen_height = windowattr.height;

		/* Update all shelf position */
		for (i = 0; i < jushelf->shelves->len; ++i) {
			jsh_shelf_update_place(g_ptr_array_index(jushelf->shelves, i));
		}
	}

	return TRUE;
}

int
main(int argc, char *argv[])
{
	JuShelf *jushelf;
	Display *disp;
	int screen;
	gint i;
	
	if (!g_module_supported()) {
		DEBUG("No GModule Support!\n");
	}

	DEBUG("Starting JuShelf\n");
	clutter_x11_set_use_argb_visual(TRUE);

	clutter_init(&argc, &argv);

	/* Get current display and screen */
	disp = clutter_x11_get_default_display();
	screen = clutter_x11_get_default_screen();

	/* Initializing Application */
	DEBUG("Initializing JuShelf\n");
	jushelf = (JuShelf *)g_slice_new(JuShelf);
	jushelf->screen_width = DisplayWidth(disp, screen);
	jushelf->screen_height = DisplayHeight(disp, screen);
	jushelf->modules = g_ptr_array_new();
	jushelf->shelves = g_ptr_array_new();
	jsh_config_file_init(jushelf);

	/* Initializing shelf */
	DEBUG("Initializing Shelves\n");
	for (i = 0; i < jushelf->shelves->len; ++i) {
		jsh_shelf_init(g_ptr_array_index(jushelf->shelves, i));
	}

	g_idle_add_full(G_PRIORITY_LOW, (GSourceFunc)jsh_event_poll, jushelf, NULL);
//	g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, jsh_event_poll, jushelf, NULL);

#if 0
	/* Create a new stage */
	stage = clutter_stage_new();
	clutter_stage_set_title(CLUTTER_STAGE(stage), "JshShelf");
	clutter_stage_set_user_resizable(CLUTTER_STAGE(stage), FALSE);
	clutter_stage_set_use_alpha(CLUTTER_STAGE(stage), TRUE);
	clutter_actor_set_opacity(stage, 0x22);
	clutter_actor_set_size(stage, 600, 64);

	/* Quit when destory */
	g_signal_connect(stage, "destroy", G_CALLBACK(clutter_main_quit), NULL);

	/* Show All */
	clutter_actor_show_all(stage);
#endif
	/* Loop */
	clutter_main();

	return EXIT_SUCCESS;
}
