#ifndef INTERNAL_JSH_WIDGET_H
#define INTERNAL_JSH_WIDGET_H

JshWidget *jsh_widget_new(JuShelf *jushelf, const gchar *module_name);
void       jsh_widget_init(JuShelf *jushelf, JshWidget *widget);
void       jsh_widget_config(JuShelf *jushelf, JshWidget *widget, JsonNode *node);

#endif
