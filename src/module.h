#ifndef JSH_MODULE_H
#define JSH_MODULE_H

JshModule *jsh_module_load(const gchar *filename);
JshModule *jsh_module_find_from_file(const gchar *module_name);
JshModule *jsh_module_find(JuShelf *jushelf, const gchar *module_name);
void       jsh_module_constructor(JshWidget *widget);
void       jsh_module_config(JshWidget *widget, JsonNode *node);

#endif
