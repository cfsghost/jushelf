#ifndef JSH_CONFIG_H
#define JSH_CONFIG_H

#include <glib.h>
#include <json-glib/json-glib.h>

typedef struct _JshConf JshConf;

JshConf  *jsh_conf_new();
gboolean  jsh_conf_load_from_file(JshConf *conf, const gchar *filename);
gint      jsh_conf_count_members(JshConf *conf, JsonNode *node);
GList    *jsh_conf_get_members(JshConf *conf, JsonNode *node, GError **error);
GList    *jsh_conf_get_nodes(JshConf *conf, JsonNode *node, GError **error);
JsonNode *jsh_conf_get_member(JshConf *conf, JsonNode *node, const gchar *key, GError **error);

#endif
