#ifndef INTERNAL_JSH_WIDGET_H
#define INTERNAL_JSH_WIDGET_H

JshWidget *jsh_widget_new(JuShelf *jushelf, const gchar *module_name);
void       jsh_widget_init(JuShelf *jushelf, JshWidget *widget, JsonNode *node);
void       jsh_widget_resize(JuShelf *jushelf, JshWidget *widget);

#endif
