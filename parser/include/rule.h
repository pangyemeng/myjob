#ifndef __RULE_H__
#define __RULE_H__
#include <pcre.h>

#define MAX_KEY 28
#define MAX_BUSINESS_KEY 20
#define MAX_NATURE_KEY 8
#define NOT_FIND "Not Find"

#define INITIAL_STAGE "\u521d\u521b\u578b"
#define GROWING_STAGE "\u6210\u957f\u578b"
#define MATURE_STAGE "\u6210\u719f\u578b"
#define DEVELOPMENT_STAGE "\u53d1\u5c55\u578b"

/* 51job rule */
#define P_NAME 1
#define C_NAME 2
#define C_BUSINESS 3
#define C_NATURE 4
#define C_SCALE 5
#define P_DATE 6
#define P_ADDRESS 7
#define P_NUMBER 8
#define P_EXPERIENCE 9
#define P_EDUCATION 10
#define P_SALARY 11
#define P_WALFARE 12
#define P_LABEL 13
#define P_FUNCTION 14
#define P_DESCRIPTION 15
#define C_INTRODUCE 16
#define C_ADDRESS 17
#define C_MAINPAGE 18
#define C_FOUND 19
#define P_RESPON_REQUIRE 20

#define JOB_MAX_RULE 19

const char job_rules[][128] = {
		"<td class=\"sr_bt\" colspan=\"\d*\" >([\\s\\S]*?)</td>",
		"<a target=\"_blank\"([\\s\\S]*?)href=\"http://search.51job.com/list/([\\s\\S]*?).html\">([\\s\\S]*?)</a>",
		"<tr><td><strong>\u516c\u53f8\u884c\u4e1a：</strong>&nbsp;&nbsp;([\\s\\S]*?)<br>",
		"<strong>\u516c\u53f8\u6027\u8d28：</strong>&nbsp;&nbsp;([\\s\\S]*?)<br>",
		"<strong>\u516c\u53f8\u89c4\u6a21：</strong>&nbsp;&nbsp;([\\s\\S]*?)</td>",
		"<td class=\"txt_1\" width=\"12%\">\u53d1\u5e03\u65e5\u671f：</td><td class=\"txt_2 \">([\\s\\S]*?)</td>",
		"<td class=\"txt_1\" width=\"12%\">\u5de5\u4f5c\u5730\u70b9：</td><td class=\"txt_2 \">([\\s\\S]*?)</td>",
		"<td class=\"txt_1\" width=\"12%\">\u62db\u8058\u4eba\u6570：</td><td class=\"txt_2 \">([\\s\\S]*?)</td>",
		"<td class=\"txt_1\" width=\"12%\">\u5de5\u4f5c\u5e74\u9650：</td><td class=\"txt_2 \">([\\s\\S]*?)</td>",
		"<td class=\"txt_1\" width=\"12%\">\u5b66([\\s\\S]*?)\u5386：</td><td class=\"txt_2 \">([\\s\\S]*?)</td>",
		"<td class=\"txt_1\" width=\"12%\">\u85aa([\\s\\S]*?)</td><td class=\"txt_2 ([\\s\\S]*?)\">([\\s\\S]*?)</td>",
		"<strong>\u85aa\u916c\u798f\u5229:</strong>([\\s\\S]*?)</span></div>",
		"<span class=\"position_label\">([\\s\\S]*?)</span>",
		"<strong>\u804c\u4f4d\u804c\u80fd:</strong>&nbsp;&nbsp;([\\s\\S]*?)</td>",
		"<strong>\u804c\u4f4d\u63cf\u8ff0([\\s\\S]*?)<div style=\"padding-bottom:30px;\">([\\s\\S]*?)</div>",
		"<p class=\"txt_font\">([\\s\\S]*?)</p>",
		"<p class=\"txt_font1\">\u5730&nbsp;&nbsp;&nbsp;&nbsp;\u5740：([\\s\\S]*?)<br>",
		"<p class=\"txt_font1\">([\\s\\S]*?)<a href=\"([\\s\\S]*?)\" target=\"_blank\">([\\s\\S]*?)</a></p>",
		"<p class=\"txt_font\">([\\s\\S]*?)\u4e8e([\\d]*?)\u5e74([\\s\\S]*?)</p>",
		"(岗位职责|工作职责)([\\s\\S]*?)(任职资格|任职要求|工作要求)([\\s\\S]*?)</div>"
};

#define MAINPAGE_RULE "www.([\\s\\S]*?).(com|cn|cc)((.(com|cn|cc))*)"
#define FOUND_RULE "\u4e8e([\\d]*?)\u5e74"

const char game_keys[][128] ={
		"\u6e38\u620f",
		"\u624b\u6e38",
		"PC\u6e38\u620f"
		};
#define GAME 3

const char server_keys[][128] ={
		"\u670d\u52a1\u5668",
		"\u8bbe\u5907"
		};
#define SERVER 2

const char cloud_keys[][128] ={
		"\u4e91\u8ba1\u7b97",
		"\u4e92\u8054\u7f51",
		"\u5927\u6570\u636e",
		"\u6258\u7ba1",
		"\u6570\u636e\u4e2d\u5fc3",
		"\u6570\u636e\u6316\u6398"
		};
#define CLOUD 6

const char finance_keys[][128] ={
		"\u8d22",
		"\u8d22\u52a1",
		"POS",
		"\u652f\u4ed8",
		"\u94f6\u884c",
		"\u79fb\u52a8",
		"\u81ea\u52a9"
		};
#define FINACE 7

const char communication_keys[][128] ={
		"\u901a\u8baf",
		"\u65e0\u7ebf",
		"AP",
		"\u5c04\u9891"
		};
#define COMMUNICATION 4

const char security_keys[][128] ={
		"\u53d6\u8bc1",
		"\u5b89\u5168\u6280\u672f",
		"\u6570\u636e\u5e93\u5b89\u5168",
		"\u5ba1\u8ba1",
		"\u52a0\u5bc6",
		"\u5b89\u5168",
		"\u7f51\u7edc\u8bbe\u5907",
		"\u9632\u4f2a",
		"\u8bc1\u4e66"};
#define SECURITY 9

const char defend_keys[][128] ={
		"\u5b89\u5168\u6280\u672f",
		"\u5b89\u9632\u8bbe\u5907",
		"\u6570\u5b57\u89c6\u9891",
		"\u5b89\u9632" };
#define DEFEND 4

const char outsource_keys[][128] ={
		"\u5916\u5305","\u51fa\u5dee" };
#define OUTSOURCE 2

const char electric_power_keys[][128] = {"\u7535\u529b"};
#define ELEC_POWER 1

const char health_keys[][128] ={
		"\u533b\u7597",
		"\u533b\u7597\u8bbe\u5907",
		"\u5668\u68b0"};
#define HEALTH 3

const char biology_keys[][128] = {"\u751f\u7269"};
#define BIOLOGY 1

const char e_commerce_keys[][128] ={
		"\u7535\u5546","\u5546\u4f1a\u7f51"};
#define E_COMMERCE 2

const char e_products_keys[][128] ={
		"\u97f3\u9891",
		"\u7535\u58f0",
		"\u7535\u5b50"};
#define E_PRODUCTS 3

const char internet_keys[][128] ={
		"\u4e92\u8054\u7f51",
		"APP",
		"\u8d27\u5e01",
		"\u865a\u62df"};
#define INTERNET 4

const char iot_keys[][128] ={
		"\u667a\u6167","\u57ce\u5e02","\u7269\u8054\u7f51"};
#define IOT 3

const char s_integrated_keys[][128] ={
		"\u4fe1\u606f",
		"\u7cfb\u7edf",
		"\u4fe1\u606f\u5316",
		"\u7cfb\u7edf\u96c6\u6210",
		"\u5b9a\u5236\u5316\u8f6f"};
#define SYS_INTERG 5

const char map_keys[][128] ={
		"\u7535\u5b50",
		"\u5730\u56fe",
		"\u5bfc\u822a"};
#define MAP 3

const char emb_keys[][128] = {"\u5d4c\u5165\u5f0f"};
#define EMB 1

const char system_keys[][128] = {"\u64cd\u4f5c\u7cfb\u7edf"};
#define SYSTEM 1

const char dev_keys[][128] ={
		"\u7535\u89c6",
		"\u673a\u9876\u76d2",
		"\u5b58\u50a8",
		"\u8ba1\u7b97\u673a\u89c6\u89c9",
		"\u8ba1\u7b97\u673a\u56fe\u50cf"
};
#define DEV 5

const char n_boards[][128] ={
		"\u521b\u4e1a\u677f",
		"A\u8f6e",
		"B\u8f6e",
		"C\u8f6e",
		"\u4e3b\u677f",
		"\u65b0\u4e09\u677f",
		"\u6c11\u8425",
		"\u56fd\u4f01"};

#define BOARD 8

typedef struct _rule
{
	pcre *pattern;
	int index;
	struct _rule *next;
}rule;

typedef struct _keywords_match
{
	char keyword[256];
	pcre *pattern;
	struct _keywords_match *next;
}keywords_match;


rule* create_rule_link();
int free_rule_link(rule *rule_head);
int create_keywords_match_link(keywords_match* keywords[]);
int free_keywords_match_link(keywords_match* keywords[]);
int print_keywords_rule(keywords_match* keywords[]);

#endif
