#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#define STR_MIN 256
#define STR_MAX 8192

typedef struct _position_info
{
	char c_name[STR_MIN];
	char p_name[STR_MIN];
	char p_date[STR_MIN];
	char p_address[STR_MIN];
	char p_number[STR_MIN];
	char p_experience[STR_MIN];
	char p_education[STR_MIN];
	char p_salary[STR_MIN];
	char p_walfare[STR_MAX];
	char p_label[STR_MIN];
	char p_function[STR_MAX];
	char p_description[STR_MAX];
	char p_responsibility[STR_MAX];
	char p_require[STR_MAX];
}position_info;

typedef struct _company_info
{
	char c_name[STR_MIN];
	char c_address[STR_MIN];
	char c_business[STR_MIN];
	char c_type[STR_MIN];
	char c_nature[STR_MIN];
	char c_found[STR_MIN];
	char c_scale[STR_MIN];
	char c_stage[STR_MIN];
	char c_brief[STR_MAX];
	char c_mainpage[STR_MIN];
}company_info;


void init_position_struct(position_info *p);
void init_company_struct(company_info *p);
int str_to_int(char *str, int len);
void print_position_info(position_info *p);
void print_company_info(company_info *p);

#endif
