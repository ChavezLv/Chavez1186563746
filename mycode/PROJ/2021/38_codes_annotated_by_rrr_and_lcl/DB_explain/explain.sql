CREATE table t1(
	id int(11) NOT NULL auto_increment,
  col1 VARCHAR(20) DEFAULT NULL,
	col2 VARCHAR(20) DEFAULT NULL,
	col3 VARCHAR(20) DEFAULT NULL,
	other_column CHAR(20) DEFAULT NULL,
	PRIMARY KEY (id),
	KEY idx_t1 (other_column),
	KEY col1_col2_col3_idx (col1,col2,col3),
	KEY col1_idx (col1)

);

CREATE table t2(
	id int(11) NOT NULL auto_increment,
	other_column CHAR(20) DEFAULT NULL,
  col1 VARCHAR(20) DEFAULT NULL,
	col2 VARCHAR(20) DEFAULT NULL,
	col3 VARCHAR(20) DEFAULT NULL,
	PRIMARY KEY (id)
);

CREATE table t3(
	id int(11) NOT NULL auto_increment,
	other_column CHAR(20) DEFAULT NULL,
  col1 VARCHAR(20) DEFAULT NULL,
	col2 VARCHAR(20) DEFAULT NULL,
	col3 VARCHAR(20) DEFAULT NULL,
	PRIMARY KEY (id)
);


explain select t2.* from t2  where id = 
				(select id from t1  where id = 
				(select t3.id from t3 where t3.other_column = ''));

explain select teacher.* from teacher  where id = 
				(select id from student  where id = 
				(select customer.id from customer where customer.name = 'qiaofeng'));

EXPLAIN select t2.* 
from t1,t2,t3
WHERE t2.id = t3.id
and 
t1.id = t2.id
and 
t1.other_column = '';


#分别显示每个表的创建过程
show create table t1;
show create table t2;
show create table t3;
show create table student;
SELECT * FROM t1;
SELECT * FROM t2;
SELECT * FROM t3;
SELECT * FROM student;

#一、查询的序号id
#id相同
explain
select * from customer,order_table 
where customer.id = order_table.user_id
and customer.id = 1;


#id有大小
explain select teacher.* from teacher  where id = 
				(select id from student  where id = 
				(select customer.id from customer where customer.name = 'qiaofeng'));

explain select * from (select * from teacher where id = 1 limit 1) tem;

#二、查询的类型select_type
explain select id from teacher where id > 1;
#PRIMARY和SUBQUERY
explain select (select id from teacher where id > 1) from student;

#显示union和union result
explain select * from teacher union select * from student;

#三、type字段，看使用索引的等级情况
#查询student 显示const system  derived
explain select * from (select * from teacher where id = 1 limit 1) tem;
explain select * from (select * from (select * from teacher where id = 1 limit 1)  tem )  wuhan;


#eq_ref
select * from order_table;
select * from customer;
explain select * from order_table,customer where customer.id = order_table.user_id;
explain select * from teacher,student where teacher.id = student.id;

#ref
explain select * from t1 where t1.other_column = 'aa';

#range
explain select * from customer where id between 1 and 4;

explain select * from customer where id in (1, 2, 3);

explain select * from customer where id > 2;

#index
explain select id from customer;
explain select id from customer where id = 1;


#ALL
explain select id from customer where age = 30;
explain select age from customer ;

#可能用到组合索引
explain select col1 from t1 where col1 = '';

#没有用到索引NULL
explain select * from customer where name = 'lili';

#四、key相关，包括可能使用索引、实际使用索引、索引长度
select * from t1;
explain  select * from t1 where col1 = 'a1' and col2 = 'b1';
explain  select * from t1 where col2 = 'b' and col1 = 'a';
explain  select * from t1 where col2 > 'a' ;
explain  select * from t1 where col1 = 'a' and col2 > 'b' ;
explain  select * from t1 where col1 = 'a' and  col2 = 'b' and col3 > 'b' ;
explain  select * from t1 where col1 = 'a1' order by col2;

#key_len = 69
explain  select * from t1 where col1 = 'a' and col2 = 'b' and col3 = 'c';

explain  select * from t1 where col1 > 'a1' and col2 > 'b' and col3 = 'c';

#key_len = 46
explain  select * from t1 where col1 = 'a' and col2 > 'b' and col3 = 'c';

#五、显示数据库的那些列被使用
#ref 显示那一列被使用
explain select * from customer, order_table where customer.id = order_table.user_id and customer.id = 1;

explain select * from t1, t2 where t1.id = t2.id and t1.other_column = 'column1';
explain select * from t1 where other_column = 'column1';

#哪些列可能被用到
explain select * from t1, t2 where t1.col1 = t2.col1 and t1.col2 = 'b1';


#六、额外信息
#using filesort
explain select col1 from t1 where col1 = 'aa' order by col3;\G
explain select col1 from t1 where col1 = 'aa' order by col1;

explain select col1 from t1 where col1 in ('a1', 'aa', 'a3', 'aaaa') group by col2\G

#using index
explain select col1,col2 from t1 where col1 = 'aa'; \G

#using temporary
explain select name from teacher where name in ('xiaohong', 'xiaowang', 'lili') group by name;\G

#uisng join buffer
explain select * from t1, t2;\G
explain select * from t1 inner join t2 on t1.other_column = t2.other_column\G

#impossible where
select * from t1;
explain select * from t1 where t1.other_column = 'wuhan' and t1.other_column = 'ww';

show create table teacher;
select * from teacher;
explain select * from teacher where id != 4;




#研究最左前缀问题
select * from t1;
explain  select * from t1 where col1 = 'a1' and col2 = 'b1';
explain  select * from t1 where col2 = 'b' and col1 = 'a';
explain  select * from t1 where col2 > 'a' ;
explain  select * from t1 where col1 = 'a' and col2 > 'b' ;
explain  select * from t1 where col1 = 'a1' order by col2;

#key_len = 69
explain  select * from t1 where col1 = 'a' and col2 = 'b' and col3 = 'c';

explain  select * from t1 where col1 > 'a1' and col2 > 'b' and col3 = 'c';

#key_len = 46
explain  select * from t1 where col1 = 'a' and col2 > 'b' and col3 = 'c';

#b后面写了%
explain  select * from t1 where col1 = 'a1' and col2 like 'b%' and col3 > 'c';
