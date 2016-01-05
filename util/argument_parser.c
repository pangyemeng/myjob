#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#include "argument_parser.h"
#include "common.h"
#include "log.h"

extern configure_file g_conf;

void parser_json(json_t *element)
{
	const char *key;
	json_t *value;
	int flag;

	json_object_foreach(element, key, value)
	{
		flag = json_typeof(value);
		if(flag == JSON_STRING && strcmp("ip", key) == 0)
		{
			snprintf(g_conf.ip, 32, "%s", json_string_value(value));
		}
		if(flag == JSON_STRING && strcmp("db_type", key) == 0)
		{
			snprintf(g_conf.db_type, 32, "%s", json_string_value(value));
		}
		if(flag == JSON_STRING && strcmp("db_name", key) == 0)
		{
			snprintf(g_conf.db_name, 32, "%s", json_string_value(value));
		}
		if(flag == JSON_STRING && strcmp("db_user", key) == 0)
		{
			snprintf(g_conf.db_user, 32, "%s", json_string_value(value));
		}
		if(flag == JSON_STRING && strcmp("db_pwd", key) == 0)
		{
			snprintf(g_conf.db_pwd, 32, "%s", json_string_value(value));
		}
		if(flag == JSON_INTEGER && strcmp("nonsql_port", key) == 0)
		{
			g_conf.nonsql_port = json_integer_value(value);
		}
		if(flag == JSON_INTEGER && strcmp("sql_port", key) == 0)
		{
			g_conf.sql_port = json_integer_value(value);
		}
		if(flag == JSON_INTEGER && strcmp("queue_max", key) == 0)
		{
			g_conf.queue_max = json_integer_value(value);
		}
		if(flag == JSON_INTEGER && strcmp("thread_max", key) == 0)
		{
			g_conf.thread_max = json_integer_value(value);
		}
		if(flag == JSON_STRING && strcmp("project_name", key) == 0)
		{
			snprintf(g_conf.project_name, 128, "%s", json_string_value(value));
		}
		if(flag == JSON_STRING && strcmp("keywords", key) == 0)
		{
			snprintf(g_conf.keywords, 256, "%s", json_string_value(value));
		}
		if(flag == JSON_STRING && strcmp("locate", key) == 0)
		{
			snprintf(g_conf.locate, 128, "%s", json_string_value(value));
		}
	}

}

/*
 * Parse text into a JSON object. If text is valid JSON, returns a
 * json_t structure, otherwise prints and error and returns null.
 */
json_t *load_json(const char *path)
{
	json_t *root;
	json_error_t error;

	root = json_load_file(path, 0, &error);

	if (root)
	{
		return root;
	}
	else
	{
		fprintf(stderr, "json error on line %d: %s\n", error.line, error.text);
		return (json_t *) 0;
	}
}

void argument_parser(const char *path)
{
	json_t *root = load_json(path);
	if (root)
	{
		/* print and release the JSON structure */
		parser_json(root);
	}
	else
	{
		LOG_ERROR("load json file error\n");
	}

}
