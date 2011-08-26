#ifndef JSH_CONF_H
#define JSH_CONF_H

#include <json-glib/json-glib.h>

struct _JshConf {
	JsonParser *parser;
	JsonNode *root;
};
typedef struct _JshConf JshConf;

#endif
