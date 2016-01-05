#include <string.h>
#include <stdio.h>

#include <debug.h>
#include "public.h"

void init_position_struct(position_info *p)
{
	memset(p->c_name, 0, sizeof(p->c_name));
	memset(p->p_name, 0, sizeof(p->p_name));
	memset(p->p_address, 0, sizeof(p->p_address));
	memset(p->p_date, 0, sizeof(p->p_date));
	memset(p->p_education, 0, sizeof(p->p_education));
	memset(p->p_experience, 0, sizeof(p->p_experience));
	memset(p->p_function, 0, sizeof(p->p_function));
	memset(p->p_label, 0, sizeof(p->p_label));
	memset(p->p_number, 0, sizeof(p->p_number));
	memset(p->p_salary, 0, sizeof(p->p_salary));
	memset(p->p_walfare, 0, sizeof(p->p_walfare));
	memset(p->p_description, 0, sizeof(p->p_description));
	memset(p->p_responsibility, 0, sizeof(p->p_responsibility));
	memset(p->p_require, 0, sizeof(p->p_require));
}

void init_company_struct(company_info *p)
{
	memset(p->c_name, 0, sizeof(p->c_name));
	memset(p->c_address, 0, sizeof(p->c_address));
	memset(p->c_found, 0, sizeof(p->c_found));
	memset(p->c_business, 0, sizeof(p->c_business));
	memset(p->c_nature, 0, sizeof(p->c_nature));
	memset(p->c_type, 0, sizeof(p->c_type));
	memset(p->c_scale, 0, sizeof(p->c_scale));
	memset(p->c_stage, 0, sizeof(p->c_stage));
	memset(p->c_brief, 0, sizeof(p->c_brief));
	memset(p->c_mainpage, 0, sizeof(p->c_mainpage));
}

int str_to_int(char *str, int len)
{
	int i, number = 0;
	for(i = 0; i < len; i++)
	{
		number = number*10 + str[i] - '0';
	}
	return number;
}

void print_position_info(position_info *p)
{
	 PRINT_WARNING("p->p_name = %s\n", p->p_name);
	 PRINT_WARNING("p->p_address = %s\n", p->p_address);
	 PRINT_WARNING("p->p_date = %s\n", p->p_date);
	 PRINT_WARNING("p->p_description = %s\n", p->p_description);
	 PRINT_WARNING("p->p_education = %s\n", p->p_education);
	 PRINT_WARNING("p->p_experience = %s\n", p->p_experience);
	 PRINT_WARNING("p->p_function = %s\n", p->p_function);
	 PRINT_WARNING("p->p_label = %s\n", p->p_label);
	 PRINT_WARNING("p->p_number = %s\n", p->p_number);
	 PRINT_WARNING("p->p_salary = %s\n", p->p_salary);
	 PRINT_WARNING("p->p_walfare = %s\n", p->p_walfare);
	 PRINT_WARNING("p->p_responsibility = %s\n", p->p_responsibility);
	 PRINT_WARNING("p->p_require = %s\n", p->p_require);
}

void print_company_info(company_info *p)
{
	 PRINT_WARNING("p->c_name = %s\n", p->c_name);
	 PRINT_WARNING("p->c_business = %s\n", p->c_business);
	 PRINT_WARNING("p->c_found = %s\n", p->c_found);
	 PRINT_WARNING("p->c_nature = %s\n", p->c_nature);
	 PRINT_WARNING("p->c_address = %s\n", p->c_address);
	 PRINT_WARNING("p->c_brief = %s\n", p->c_brief);
	 PRINT_WARNING("p->c_scale = %s\n", p->c_scale);
	 PRINT_WARNING("p->c_stage = %s\n", p->c_stage);
	 PRINT_WARNING("p->c_mainpage = %s\n", p->c_mainpage);
}
