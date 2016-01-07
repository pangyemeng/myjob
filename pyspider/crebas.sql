/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     2015/6/2 20:02:07                            */
/*==============================================================*/


drop table if exists company;

drop table if exists position;

/*==============================================================*/
/* Table: company                                               */
/*==============================================================*/
create table company
(
   c_id                 int not null,
   c_name               varchar(256) not null,
   c_type               varchar(256),
   c_scale              varchar(128),
   c_brief              varchar(4096) not null,
   c_product            varchar(2048),
   c_homepage           varchar(128) not null,
   c_evaluates          varchar(4069),
   primary key (c_id)
);

alter table company comment '公司实体：公司名称、公司性质、公司规模、公司简介、公司产品介绍、公司主页、公司评价';

/*==============================================================*/
/* Table: position                                              */
/*==============================================================*/
create table position
(
   p_id                 int not null,
   c_id                 int not null,
   p_date               date not null,
   p_address            varchar(1024) not null,
   p_number             int,
   p_experience         varchar(128),
   p_education          varchar(128),
   p_salary             varchar(128),
   p_welfare            varchar(4096),
   p_label              varchar(1024),
   p_function           varchar(2048) not null,
   p_description        varchar(4096) not null,
   p_quality            varchar(4096),
   primary key (p_id)
);

alter table position comment '招聘职位信息：发布日期、工作地点、招聘人数、工作年限、学历、薪水范围、薪酬福利、职位标签、职位职能、职位描述、其他基本素';

alter table position add constraint FK_c_to_p foreign key (c_id)
      references company (c_id) on delete restrict on update restrict;

