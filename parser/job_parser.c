#include <unistd.h>
#include <hiredis.h>
#include <pcre.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zdb.h>
#include <sys/prctl.h>

#include "public.h"
#include "job_parser.h"
#include "redis_db.h"
#include "log.h"
#include "debug.h"
#include "rule.h"
#include "html_util.h"
#include "sql_db.h"
#include "common.h"

#define OVECCOUNT 255

const char q_name[] = "job";

void handle_51job_info(char *str, int length, rule *rule_head, keywords_match* keywords[], Connection_T conn);
static void parser_company_name(char *dest, char *start, int ovector[]);
static void parser_position_name(char *dest, char *start, int ovector[]);
static void parser_company_industry(char *dest, char *start, int ovector[]);
static void parser_company_nature(char *dest, char *start, int ovector[]);
static void parser_company_scale(char *dest, char *start, int ovector[]);
static void parser_position_date(char *dest, char *start, int ovector[]);
static void parser_position_address(char *dest, char *start, int ovector[]);
static void parser_position_number(char *dest, char *start, int ovector[]);
static void parser_position_experience(char *dest, char *start, int ovector[]);
static void parser_position_education(char *dest, char *start, int ovector[]);
static void parser_position_salary(char *dest, char *start, int ovector[]);
static void parser_position_walfare(char *dest, char *start, int ovector[]);
static void parser_position_label(char *dest, char *start, int ovector[]);
static void parser_position_function(char *dest, char *start, int ovector[]);
static void parser_position_description(char *dest, char *start, int ovector[]);
static void parser_company_introduce(char *dest, char *start, int ovector[]);
static void parser_company_address(char *dest, char *start, int ovector[]);
static void parser_company_mainpage(char *dest, char *start, int ovector[]);
static void parser_company_found(char *dest, char *start, int ovector[]);
static void parser_position_respon_require(position_info *p_info, char *start, int ovector[]);
static void further_fetch_company_info(company_info *c_info, keywords_match* keywords[]);
static int further_fetch_company_stage(char *c_scale, char *c_stage);
static int further_company_mainpage(char *str, int len, char *time);
static int further_company_found(char *str, int len, char *dest);
static int fetch_number(char *c_scale);
static void save_company_info_to_db(company_info *c_info, Connection_T conn);
static void save_position_info_to_db(position_info *p, Connection_T conn);
static void position_mutual_company_convert(company_info *c_info, position_info *p_info);
static void further_fetch_position_info(position_info *p_info);
static void postion_data_clean(position_info *p_info);
static void walfare_clean(char *str);

void *job_parser(void *arg)
{
	char *thread_name = "Job_Parser_%d";
	char name[128] = { 0 };
	database db = MYJOB_DB;
	redisContext *context;
	redisReply *reply = NULL;
	rule *rule_head = NULL;
	keywords_match* keywords_rule[MAX_KEY];
	Connection_T conn = NULL;

	/* thread name */
	snprintf(name, 128, thread_name, (int *)arg);
	prctl(PR_SET_NAME, (unsigned long)name);

	context = redis_connect(g_conf.ip, g_conf.nonsql_port);
	if(NULL == context)
	{
		return NULL;
	}
	rule_head = create_rule_link();
	if(NULL == rule_head)
	{
		PRINT_ERROR("Create Rule Link Fail\n");
		LOG_ERROR("Create Rule Link Fail\n");
		return NULL;
	}
	create_keywords_match_link(keywords_rule);
	if(NULL == keywords_rule)
	{
		PRINT_ERROR("Create Keywords Rule Link Fail\n");
		LOG_ERROR("Create Keywords Rule Link Fail\n");
		return NULL;
	}
	conn = sql_db_connect(db);
//	print_keywords_rule(keywords_rule);

	while (1)
	{
		reply = pop(context, q_name);
		//PRINT_INFO("len = %d, str = %s\n", reply->len, reply->str);
		handle_51job_info(reply->str, reply->len, rule_head, keywords_rule, conn);
		freeReplyObject(reply);
		reply = NULL;
	}
	Connection_close(conn);
}

void handle_51job_info(char *str, int length, rule *rule_head, keywords_match* keywords[], Connection_T conn)
{
	PRINT_DEBUG("Handle 51 job\n");
	char *start = str;
	rule *tmp = rule_head;
	int ovector[OVECCOUNT];

	/* position information */
	position_info *position = (position_info *) malloc(sizeof(position_info));
	if (position == NULL)
	{
		LOG_ERROR("Malloc Fail\n");
		return;
	}
	init_position_struct(position);

	/* company information */
	company_info *company = (company_info *) malloc(sizeof(company_info));
	if (company == NULL)
	{
		LOG_ERROR("Malloc Fail\n");
		free(position);
		position = NULL;
		return;
	}
	init_company_struct(company);
//	PRINT_INFO("%s\n", start);

	while (tmp->next != NULL)
	{
		if (pcre_exec(tmp->pattern, NULL, (char *) start, length, 0, 0,
				ovector, OVECCOUNT) >= 0)
		{
//			PRINT_INFO("type = %d\n", tmp->index);
			switch (tmp->index)
			{
			case C_NAME:
				parser_company_name(company->c_name, start, ovector);
				break;
			case C_BUSINESS:
				parser_company_industry(company->c_business, start, ovector);
				break;
			case C_NATURE:
				parser_company_nature(company->c_nature, start, ovector);
				break;
			case C_SCALE:
				parser_company_scale(company->c_scale, start, ovector);
				break;
			case C_INTRODUCE:
				parser_company_introduce(company->c_brief, start, ovector);
				break;
			case C_ADDRESS:
				parser_company_address(company->c_address, start, ovector);
				break;
			case C_MAINPAGE:
				parser_company_mainpage(company->c_mainpage, start, ovector);
				break;
			case C_FOUND:
				parser_company_found(company->c_found, start, ovector);
				break;
			case P_NAME:
				parser_position_name(position->p_name, start, ovector);
				break;
			case P_DATE:
				parser_position_date(position->p_date, start, ovector);
				break;
			case P_ADDRESS:
				parser_position_address(position->p_address, start, ovector);
				break;
			case P_NUMBER:
				parser_position_number(position->p_number, start, ovector);
				break;
			case P_EXPERIENCE:
				parser_position_experience(position->p_experience, start,
						ovector);
				break;
			case P_EDUCATION:
				parser_position_education(position->p_education, start, ovector);
				break;
			case P_SALARY:
				parser_position_salary(position->p_salary, start, ovector);
				break;
			case P_WALFARE:
				parser_position_walfare(position->p_walfare, start, ovector);
				break;
			case P_LABEL:
				parser_position_label(position->p_label, start, ovector);
				break;
			case P_FUNCTION:
				parser_position_function(position->p_function, start, ovector);
				break;
			case P_DESCRIPTION:
				parser_position_description(position->p_description, start,
						ovector);
				break;
			case P_RESPON_REQUIRE:
				parser_position_respon_require(position, start, ovector);
				break;
			default:
				PRINT_DEBUG("Don't Contain info\n");
				break;
			}
		}
		tmp = tmp->next;
	}
	further_fetch_company_info(company, keywords);
	position_mutual_company_convert(company, position);
	further_fetch_position_info(position);

	if(position->p_name[0] == '\0')
	{
		if(pcre_exec(rule_head->pattern, NULL, (char *) start, length, 0, 0,
						ovector, OVECCOUNT) >= 0)
		{
			parser_position_name(position->p_name, start, ovector);
		}

	}
	//print_position_info(position);
	//print_company_info(company);

	save_company_info_to_db(company, conn);
	save_position_info_to_db(position, conn);

	/* memory free */
	free(company);
	company = NULL;
	free(position);
	position = NULL;
}

void position_mutual_company_convert(company_info *c_info, position_info *p_info)
{
	//company address
	if(c_info->c_address[0] == '\0')
	{
		memcpy(c_info->c_address, p_info->p_address, strlen(p_info->p_address));
	}
	//company name for table position
	if(p_info->c_name[0] == '\0')
	{
		memcpy(p_info->c_name, c_info->c_name, strlen(c_info->c_name));
	}
}

void further_fetch_position_info(position_info *p_info)
{
	const char default_number[] = "若干";
	const char default_experince[] = "无要求";
	const char default_salary[] = "面议";
	const char default_education[] = "无要求";

	if(p_info->p_number[0] == '\0')
	{
		memcpy(p_info->p_number, default_number, strlen(default_number));
	}
	if(p_info->p_experience[0] == '\0')
	{
		memcpy(p_info->p_experience, default_experince, strlen(default_experince));
	}
	if(p_info->p_salary[0] == '\0')
	{
		memcpy(p_info->p_salary, default_salary, strlen(default_salary));
	}
	if(p_info->p_education[0] == '\0')
	{
		memcpy(p_info->p_education, default_education, strlen(default_education));
	}
}

void further_fetch_company_info(company_info *c_info, keywords_match* keywords[])
{
	int i;
	keywords_match* tmp = NULL;
	int ovector[OVECCOUNT];
	char business_buf[256] = { 0 };
	char type_buf[256] = { 0 };
	char blank[] = " ";

	//company business
	if(c_info->c_business[0] == '\0')
	{
		for(i = 0; i < MAX_BUSINESS_KEY; i++)
		{
			tmp = keywords[i];
			while(tmp != NULL)
			{
				//brief
				if(pcre_exec(tmp->pattern, NULL, c_info->c_brief, strlen(c_info->c_brief), 0, 0, ovector, OVECCOUNT) >= 0)
				{
					strcat(business_buf, tmp->keyword);
					strcat(business_buf, blank);
				}
				tmp = tmp->next;
			}
		}
	}
	memcpy(c_info->c_business, business_buf, strlen(business_buf));

	//nature , c_type
	if(c_info->c_nature[0] != '\0')
	{
		strcat(type_buf, c_info->c_nature);
		strcat(type_buf, blank);
	}
	for(i = 0 ; i < MAX_NATURE_KEY; i++)
	{
		tmp = keywords[MAX_BUSINESS_KEY + i];
		while(tmp != NULL)
		{
			//brief,nature
			if(pcre_exec(tmp->pattern, NULL, c_info->c_brief, strlen(c_info->c_brief), 0, 0, ovector, OVECCOUNT) >= 0)
			{
				strcat(type_buf, c_info->c_nature);
				strcat(type_buf, blank);
			}
			tmp = tmp->next;
		}
	}
	memcpy(c_info->c_type, type_buf, strlen(type_buf));

	//company found
	if(c_info->c_found[0] == '\0')
	{
		further_company_found(c_info->c_brief, strlen(c_info->c_brief), c_info->c_found);
	}
	//mainpage
	if(c_info->c_mainpage[0] == '\0')
	{
		further_company_mainpage(c_info->c_brief, strlen(c_info->c_brief), c_info->c_mainpage);
	}
	//stage
	further_fetch_company_stage(c_info->c_scale, c_info->c_stage);

	//null process
	if(c_info->c_found[0] == '\0')
	{
		memcpy(c_info->c_found, NOT_FIND, sizeof(NOT_FIND));
	}
	if(c_info->c_mainpage[0] == '\0')
	{
		memcpy(c_info->c_mainpage, NOT_FIND, sizeof(NOT_FIND));
	}
}

int further_fetch_company_stage(char *c_scale, char *c_stage)
{
	int number = 0;
	number = fetch_number(c_scale);
	// number <= 50 is The initial stage
	if(number <= 50)
	{
		memcpy(c_stage, INITIAL_STAGE, sizeof(INITIAL_STAGE));
	}
	// 50 < number <= 150 Growing up
	if(number > 50 && number <= 150)
	{
		memcpy(c_stage, GROWING_STAGE, sizeof(GROWING_STAGE));
	}
	// 150 < number <= 500 Mature
	if(number > 150 && number <= 500)
	{
		memcpy(c_stage, MATURE_STAGE, sizeof(MATURE_STAGE));
	}
	// number > 500 Development
	if(number > 500)
	{
		memcpy(c_stage, DEVELOPMENT_STAGE, sizeof(DEVELOPMENT_STAGE));
	}
	return 1;
}

int fetch_number(char *c_scale)
{
	int number = 0;
	int number_min = 0;
	int number_max = 0;
	int i;
	char min[5] = { 0 };
	int min_foot = 0;
	char max[5] = { 0 };
	int max_foot = 0;
	int interval = 0;

	for(i = 0; c_scale[i] != '\0'; i++)
	{
		if(c_scale[i] >= '0' && c_scale[i] <= '9' && !interval)
		{
			min[min_foot++] = c_scale[i];
		}
		if(c_scale[i] >= '0' && c_scale[i] <= '9' && interval)
		{
			max[max_foot++] = c_scale[i];
		}
		if(c_scale[i] == '-')
		{
			interval = 1;
		}
	}

	number_min = str_to_int(min, min_foot);
	number = number_min;
	if(interval)
	{
		number_max = str_to_int(max, max_foot);
		number = (number_max + number_min) / 2;
	}
	return number;
}

int further_company_found(char *str, int len, char *dest)
{
	pcre *c_found = NULL;
	const char *error;
	int erroffset;
	int ovector[OVECCOUNT];
	char *sub_start = NULL;
	int str_len;

	//found
	c_found = pcre_compile(FOUND_RULE, 0, &error, &erroffset, NULL);
	if (c_found == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return 0 ;
	}

	if(pcre_exec(c_found, NULL, str, len, 0, 0, ovector, OVECCOUNT) >= 0)
	{
		sub_start = (char *) str + ovector[2 * 1];
		str_len = ovector[2 * 1 + 1] - ovector[2 * 1];
		if(str_len > 20)
		{
			return 0;
		}
		memcpy(dest, sub_start, str_len);
		return 1;
	}
	return 0;
}

int further_company_mainpage(char *str, int len, char *dest)
{
	pcre *c_mainpage = NULL;
	const char *error;
	int erroffset;
	int ovector[OVECCOUNT];
	char *sub_start = NULL;
	int str_len;

	//mainpage
	c_mainpage = pcre_compile(MAINPAGE_RULE, 0, &error, &erroffset, NULL);
	if (c_mainpage == NULL)
	{
		PRINT_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		LOG_ERROR("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return 0 ;
	}

	if(pcre_exec(c_mainpage, NULL, str, len, 0, 0, ovector, OVECCOUNT) >= 0)
	{
		sub_start = (char *) str + ovector[2 * 0];
		str_len = ovector[2 * 0 + 1] - ovector[2 * 0];
		if(str_len > 20)
		{
			return 0;
		}
		memcpy(dest, sub_start, str_len);
		//PRINT_INFO("company mainpage = %s\n", dest);
		return 1;
	}
	return 0;
}

void save_position_info_to_db(position_info *p_info, Connection_T conn)
{
	char sql_position[] = "INSERT INTO position (c_name, p_name, p_date, p_address, p_number, p_experience, p_education, p_salary, p_welfare, p_label, p_function, p_description, p_respon, p_require) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
	char sql_select_id[] = "SELECT c_id FROM company where c_name = ?;";
	PreparedStatement_T p = NULL;
	ResultSet_T result = NULL;

	TRY
	{
		/*1、select company id*/
		p = Connection_prepareStatement(conn, sql_select_id);
		PreparedStatement_setString(p, 1, p_info->c_name);
		result = PreparedStatement_executeQuery(p);
		while (ResultSet_next(result))
		{
			return;
		}
		//data process
		postion_data_clean(p_info);
		/* insert delete update*/
		p = Connection_prepareStatement(conn, sql_position);
		PreparedStatement_setString(p, 1, p_info->c_name);
		PreparedStatement_setString(p, 2, p_info->p_name);
		PreparedStatement_setString(p, 3, p_info->p_date);
		PreparedStatement_setString(p, 4, p_info->p_address);
		PreparedStatement_setString(p, 5, p_info->p_number);
		PreparedStatement_setString(p, 6, p_info->p_experience);
		PreparedStatement_setString(p, 7, p_info->p_education);
		PreparedStatement_setString(p, 8, p_info->p_salary);
		PreparedStatement_setString(p, 9, p_info->p_walfare);
		PreparedStatement_setString(p, 10, p_info->p_label);
		PreparedStatement_setString(p, 11, p_info->p_function);
		PreparedStatement_setString(p, 12, p_info->p_description);
		PreparedStatement_setString(p, 14, p_info->p_responsibility);
		PreparedStatement_setString(p, 15, p_info->p_require);
		PreparedStatement_execute(p);
	}
	CATCH(SQLException)
	{
			LOG_ERROR("SQLException -- %s\n", Exception_frame.message);
	}
	END_TRY;
	LOG_TRACE("Insert Position info into db\n");
	PRINT_INFO("Insert Position info into db\n");
}

void save_company_info_to_db(company_info *c_info, Connection_T conn)
{
	char sql_company[] = "INSERT  INTO company(c_name, c_address, c_business, c_type, c_found, c_scale, c_stage, c_brief, c_homepage)  VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";
	char sql_select_id[] = "SELECT c_id FROM company where c_name = ?;";
	ResultSet_T result = NULL;
	PreparedStatement_T p = NULL;
	//data process

	TRY
	{
		/*1、select company id*/
		p = Connection_prepareStatement(conn, sql_select_id);
		PreparedStatement_setString(p, 1, c_info->c_name);
		result = PreparedStatement_executeQuery(p);
		while (ResultSet_next(result))
		{
			return;
		}
		/* insert delete update*/
		p = Connection_prepareStatement(conn, sql_company);
		PreparedStatement_setString(p, 1, c_info->c_name);
		PreparedStatement_setString(p, 2, c_info->c_address);
		PreparedStatement_setString(p, 3, c_info->c_business);
		PreparedStatement_setString(p, 4, c_info->c_type);
		PreparedStatement_setString(p, 5, c_info->c_found);
		PreparedStatement_setString(p, 6, c_info->c_scale);
		PreparedStatement_setString(p, 7, c_info->c_stage);
		PreparedStatement_setString(p, 8, c_info->c_brief);
		PreparedStatement_setString(p, 9, c_info->c_mainpage);
		PreparedStatement_execute(p);
	}
	CATCH(SQLException)
	{
			LOG_ERROR("SQLException -- %s\n", Exception_frame.message);
	}
	END_TRY;
	LOG_TRACE("Insert Company info into db\n");
	PRINT_INFO("Insert Company info into db\n");
}

void postion_data_clean(position_info *p_info)
{
	if(p_info->p_walfare[0] != '\0')
	{
		walfare_clean(p_info->p_walfare);
	}
	if(p_info->p_function[0] != '\0')
	{
		walfare_clean(p_info->p_function);
	}
}

void walfare_clean(char *str)
{
	int i;
	char dest[STR_MAX] = { 0 };
	int indx = 0;
	int statu1 = 0, statu2 = 0;
	int flag = 0;

	for(i = 0; i < strlen(str); i++)
	{
		if((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 0 && str[i] <= 177))
		{
			statu2 = 1;
			continue;
		}
		if(statu1 == statu2){
			dest[indx++] = str[i];
		}
		else
		{
			if(!flag)
			{
				dest[indx++] = str[i];
				statu1 = 0;
				statu2 = 0;
				flag = 1;
			}
			else
			{
				dest[indx++] = ',';
				dest[indx++] = str[i];
				statu1 = 0;
				statu2 = 0;
			}
		}
	}
	memcpy(str, dest, sizeof(dest));
	PRINT_DEBUG("werfare %s\n", str);
}

void parser_company_found(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 2];
	len = ovector[2 * 2 + 1] - ovector[2 * 2];
	if(len > 10)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("company_found = %s\n", dest);
}

void parser_company_introduce(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];

	if(len > STR_MAX)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("company_introduce = %s\n", dest);
}

void parser_company_nature(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("company natrue = %s\n", dest);
}

void parser_company_scale(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("company scale = %s\n", dest);
}
void parser_position_date(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("postion date = %s\n", dest);
}
void parser_position_address(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("position_address = %s\n", dest);
}
void parser_position_number(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("position_number = %s\n", dest);
}
void parser_position_experience(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("position_experience = %s\n", dest);
}
void parser_position_education(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 2];
	len = ovector[2 * 2 + 1] - ovector[2 * 2];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("position_education = %s\n", dest);
}
void parser_position_salary(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 3];
	len = ovector[2 * 3 + 1] - ovector[2 * 3];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("position_salary = %s\n", dest);
}
void parser_position_walfare(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];
	if(len > STR_MAX)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("position_walfare = %s\n", dest);
}
void parser_position_label(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("position_label = %s\n", dest);
}
void parser_position_function(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];
	if(len > STR_MAX)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("position_function = %s\n", dest);
}
void parser_position_description(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 2];
	len = ovector[2 * 2 + 1] - ovector[2 * 2];
	if(len > STR_MAX)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("position_description = %s\n", dest);
}

void parser_position_respon_require(position_info *p_info, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len = 0;
	//responsibility
	sub_start = (char *) start + ovector[2 * 2];
	len = ovector[2 * 2 + 1] - ovector[2 * 2];
	if(len > STR_MAX)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(p_info->p_responsibility , sub_start, len);
	PRINT_DEBUG("responsibility = %s\n", p_info->p_responsibility);
	//require
	sub_start = NULL;
	len = 0;
	sub_start = (char *) start + ovector[2 * 4];
	len = ovector[2 * 4 + 1] - ovector[2 * 4];
	if(len > STR_MAX)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(p_info->p_require , sub_start, len);
	PRINT_DEBUG("require = %s\n", p_info->p_require);
}
void parser_company_address(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("company_address = %s\n", dest);
}
void parser_company_mainpage(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 3];
	len = ovector[2 * 3 + 1] - ovector[2 * 3];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("company_mainpage = %s\n", dest);
}

void parser_company_industry(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("company industry = %s\n", dest);
}

void parser_position_name(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 1];
	len = ovector[2 * 1 + 1] - ovector[2 * 1];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("position name = %s\n", dest);
}

void parser_company_name(char *dest, char *start, int ovector[])
{
	char *sub_start = NULL;
	int len;
	sub_start = (char *) start + ovector[2 * 3];
	len = ovector[2 * 3 + 1] - ovector[2 * 3];
	if(len > STR_MIN)
	{
		PRINT_ERROR("Len Is Big\n");
		return;
	}
	memcpy(dest, sub_start, len);
	PRINT_DEBUG("company name = %s\n", dest);
}
