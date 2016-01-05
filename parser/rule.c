#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"
#include "debug.h"
#include "rule.h"

int free_rule_link(rule *rule_head)
{
	rule *head = rule_head;
	rule *tmp = NULL;

	while (head->next != NULL)
	{
		tmp = head;
		if (tmp->pattern != NULL)
		{
			pcre_free(tmp->pattern);
		}
		head = head->next;
		free(tmp);
	}
	return 1;
}

rule* create_rule_link()
{
	const char *error;
	int erroffset;
	rule *tmp = NULL;
	rule *current = NULL;
	int loop = sizeof(job_rules) / sizeof(job_rules[0]);
	int i = 0;
	rule *rule_head = NULL;

//	PRINT_INFO("loop = %d\n", loop);
	tmp = (rule *) malloc(sizeof(rule));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(job_rules[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		free_rule_link(rule_head);
		return NULL;
	}
	tmp->next = NULL;
	tmp->index = 1;
	rule_head = tmp;
	current = tmp;

	for (i = 1; i < loop; i++)
	{
		tmp = (rule *) malloc(sizeof(rule));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(job_rules[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_rule_link(rule_head);
			return NULL;
		}
		tmp->next = NULL;
		tmp->index = i + 1;
		current->next = tmp;
		current = tmp;
	}
	return rule_head;
}

int create_keywords_match_link(keywords_match* keywords[])
{
	const char *error;
	int erroffset;
	keywords_match* tmp = NULL, *current = NULL;
	int i = 0;
	int index = 0;

	for(i = 0; i < MAX_KEY; i++)
	{
		keywords[i] = NULL;
	}

	//game
	PRINT_INFO("Init Game Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(game_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, game_keys[0], sizeof(game_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < GAME; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(game_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, game_keys[i], sizeof(game_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//server
	PRINT_INFO("Init Server Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(server_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, server_keys[0], sizeof(server_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < SERVER; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(server_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, server_keys[i], sizeof(server_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//cloud
	PRINT_INFO("Init Cloud Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(cloud_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, cloud_keys[0], sizeof(cloud_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < CLOUD; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(cloud_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, cloud_keys[i], sizeof(cloud_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//FINANCIAL
	PRINT_INFO("Init Financial Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(finance_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, finance_keys[0], sizeof(finance_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < FINACE; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(finance_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, finance_keys[i], sizeof(finance_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//communication_keys
	PRINT_INFO("Init Communication Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(communication_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, communication_keys[0], sizeof(communication_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < COMMUNICATION; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(communication_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, communication_keys[i], sizeof(communication_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//security_keys
	PRINT_INFO("Init Security Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(security_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, security_keys[0], sizeof(security_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < SECURITY; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(security_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, security_keys[i], sizeof(security_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//defend_keys
	PRINT_INFO("Init Defend Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(defend_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, defend_keys[0], sizeof(defend_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < DEFEND; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(defend_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, defend_keys[i], sizeof(defend_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//outsource_keys
	PRINT_INFO("Init Outsource Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(outsource_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, outsource_keys[0], sizeof(outsource_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < OUTSOURCE; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(outsource_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, outsource_keys[i], sizeof(outsource_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//electric_power_keys
	PRINT_INFO("Init Electric Power Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(electric_power_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, electric_power_keys[0], sizeof(electric_power_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < ELEC_POWER; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(electric_power_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, electric_power_keys[0], sizeof(electric_power_keys[0]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//health_keys
	PRINT_INFO("Init Health Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(health_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, health_keys[0], sizeof(health_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < HEALTH; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(health_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, health_keys[i], sizeof(health_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//biology_keys
	PRINT_INFO("Init Biology Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(biology_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, biology_keys[0], sizeof(biology_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < BIOLOGY; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(biology_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, biology_keys[i], sizeof(biology_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//e_commerce_keys
	PRINT_INFO("Init E_Commerce Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(e_commerce_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, e_commerce_keys[0], sizeof(e_commerce_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < E_COMMERCE; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(e_commerce_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, e_commerce_keys[i], sizeof(e_commerce_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//e_products_keys
	PRINT_INFO("Init E_Products Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(e_products_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, e_products_keys[0], sizeof(e_products_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < E_PRODUCTS; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(e_products_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, e_products_keys[i], sizeof(e_products_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//internet_keys
	PRINT_INFO("Init Internet Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(internet_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, internet_keys[0], sizeof(internet_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < INTERNET; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(internet_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, internet_keys[i], sizeof(internet_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//iot_keys
	PRINT_INFO("Init IOT Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(iot_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, iot_keys[0], sizeof(iot_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < IOT; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(iot_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, iot_keys[i], sizeof(iot_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//s_integrated_keys
	PRINT_INFO("Init System Integrated Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(s_integrated_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, s_integrated_keys[0], sizeof(s_integrated_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < SYS_INTERG; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(s_integrated_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, s_integrated_keys[i], sizeof(s_integrated_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//map_keys
	PRINT_INFO("Init Map Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(map_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, map_keys[0], sizeof(map_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < MAP; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(map_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, map_keys[i], sizeof(map_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//emb_keys
	PRINT_INFO("Init Emb Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(emb_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, emb_keys[0], sizeof(emb_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < EMB; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(emb_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, emb_keys[i], sizeof(emb_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//system_keys
	PRINT_INFO("Init System Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(system_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, system_keys[0], sizeof(system_keys[0]));
	tmp->next = NULL;
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < SYSTEM; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(system_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, system_keys[i], sizeof(system_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//dev_keys
	PRINT_INFO("Init Dev Keywords\n");
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(dev_keys[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, dev_keys[0], sizeof(dev_keys[0]));
	keywords[index++] = tmp;
	current = tmp;
	for(i = 1; i < DEV; i++)
	{
		tmp = (keywords_match *) malloc(sizeof(keywords_match));
		if (tmp == NULL)
		{
			PRINT_ERROR("Malloc Fail\n");
			LOG_ERROR("Malloc Fail\n");
			return NULL;
		}
		tmp->pattern = pcre_compile(dev_keys[i], 0, &error, &erroffset, NULL);
		if (tmp->pattern == NULL)
		{
			PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
			free_keywords_match_link(keywords);
			return NULL;
		}
		memset(tmp->keyword, 0, sizeof(tmp->keyword));
		memcpy(tmp->keyword, dev_keys[i], sizeof(dev_keys[i]));
		tmp->next = NULL;
		current->next = tmp;
		current = tmp;
	}
	//Second-board Market
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(n_boards[0], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, n_boards[0], sizeof(n_boards[0]));
	keywords[index++] = tmp;

	//A
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(n_boards[1], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, n_boards[1], sizeof(n_boards[1]));
	keywords[index++] = tmp;

	//B
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(n_boards[2], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, n_boards[2], sizeof(n_boards[2]));
	keywords[index++] = tmp;

	//C
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(n_boards[3], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, n_boards[3], sizeof(n_boards[3]));
	keywords[index++] = tmp;

	//N_M_BOARD_M
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(n_boards[4], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, n_boards[4], sizeof(n_boards[4]));
	keywords[index++] = tmp;

	//N_N_BOARD_M
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(n_boards[5], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, n_boards[5], sizeof(n_boards[5]));
	keywords[index++] = tmp;

	//N_P_E
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(n_boards[6], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, n_boards[6], sizeof(n_boards[6]));
	keywords[index++] = tmp;
	//N_SOE
	tmp = (keywords_match *) malloc(sizeof(keywords_match));
	if (tmp == NULL)
	{
		PRINT_ERROR("Malloc Fail\n");
		LOG_ERROR("Malloc Fail\n");
		return NULL;
	}
	tmp->pattern = pcre_compile(n_boards[7], 0, &error, &erroffset, NULL);
	if (tmp->pattern == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return NULL;
	}
	memset(tmp->keyword, 0, sizeof(tmp->keyword));
	memcpy(tmp->keyword, n_boards[7], sizeof(n_boards[7]));
	keywords[index++] = tmp;

	return 0;
}

int free_keywords_match_link(keywords_match* keywords[])
{
	return 0;
}

int print_keywords_rule(keywords_match* keywords[])
{
	int i, count;
	keywords_match *tmp = NULL;
	for(i = 0; i < MAX_KEY; i++)
	{
		count = 0;
		tmp = keywords[i];
		while(tmp)
		{
			count++;
			printf("%s\n", tmp->keyword);
			tmp = tmp->next;
		}
		printf("code = %s,number = %d\n",keywords[i]->keyword, count);
	}
	return 0;
}
