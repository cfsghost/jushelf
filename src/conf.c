#include <stdlib.h>
#include <glib.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <sys/stat.h>
#include <jushelf/conf.h>
#include "jushelf.h"
#include "conf.h"

JshConf *
jsh_conf_new()
{
	JshConf *conf;

	conf = (JshConf *)g_slice_new0(JshConf);
	conf->parser = json_parser_new();

	return conf;
}

gboolean
jsh_conf_load_from_file(JshConf *conf, const gchar *filename)
{
	GError *error = NULL;

	DEBUG("Loading config file: %s\n", filename);
	if (!json_parser_load_from_file(conf->parser, filename, &error)) {
		g_error("Unable to read file\n%s\n", error->message);
		return FALSE;
	}

	DEBUG("Parsing config file\n", filename);
	conf->root = json_parser_get_root(conf->parser);

	return TRUE;
}

gint
jsh_conf_count_members(JshConf *conf, JsonNode *node)
{
	JsonObject *object;

	/* We consider it is a object */
	if (node)
		object = json_node_get_object(node);
	else
		object = json_node_get_object(conf->root);

	return json_object_get_size(object);
}

GList *
jsh_conf_get_members(JshConf *conf, JsonNode *node, GError **error)
{
	JsonObject *object;

	/* We consider it is a object */
	if (node)
		object = json_node_get_object(node);
	else
		object = json_node_get_object(conf->root);

	return json_object_get_members(object);
}

GList *
jsh_conf_get_nodes(JshConf *conf, JsonNode *node, GError **error)
{
	JsonObject *object;

	/* We consider it is a object */
	if (node)
		object = json_node_get_object(node);
	else
		object = json_node_get_object(conf->root);

	return json_object_get_values(object);
}

JsonNode *
jsh_conf_get_member(JshConf *conf, JsonNode *node, const gchar *key, GError **error)
{
	JsonObject *object;

	/* We consider it is a object */
	if (node)
		object = json_node_get_object(node);
	else
		object = json_node_get_object(conf->root);

	return json_object_get_member(object, key);
}
