 ///
 /// @file    mysqltest.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-10-08 14:52:58
 ///
 
#include <head.h>
#include <mysql/mysql.h>

// 安装mysql的C语言开发客户端组件
// sudo apt install libmysqlclient-dev
 
int main(int argc, char *argv[])
{
	MYSQL * conn = mysql_init(NULL);
 
	const char * host = "localhost";
	const char * user = "root";
	const char * passwd = "1234";
	const char * dbname = "46th";

	//mysqlserver的默认端口是3306
	//完成三次握手，同时对用户和密码进行验证
	if(!mysql_real_connect(conn, host, user, passwd, dbname, 0, NULL, 0)) {
		//处理出错的情况
		printf("%s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}

	//设置客户端的字符集,才能在客户端正常显示中文
	char * chinese = "set names 'utf8'";
	if(mysql_real_query(conn, chinese, strlen(chinese))) {
		printf("%s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}
	
	//开始查询操作
	char * query = "select * from student";
	if(mysql_real_query(conn, query, strlen(query))) {
		printf("%s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}

	//查询完成之后，要获取结果集，
	//使用mysql_store_result会立刻获取结果集
	MYSQL_RES * res = mysql_store_result(conn);

	//获取每一行
	if(res) {
		MYSQL_ROW row;// char **
		int rows = mysql_num_rows(res);// 0
		int cols = mysql_num_fields(res);
		printf("rows: %d, cols: %d\n", rows, cols);
		while((row = mysql_fetch_row(res)) != NULL) {
			//获取每一行的每一列
			for(int i = 0; i < cols; ++i) {
				printf("%15s", row[i]);
			}
			printf("\n");
		}
	}

	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}
