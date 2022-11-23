#include <mysql/mysql.h>
#include <string.h>
#include <stdio.h>
//首先我们要包含mysql的头文件，并链接mysql动态库。即添加以下语句：
//#include <WinSock2.h> // 进行网络编程需要winsock2.h
//   #include <mysql.h>
// #pragma comment（lib， “libmysql.lib”）
int main(int argc,char* argv[])
{
	MYSQL *conn;//创建MYSQL变量
	char *server = "localhost";
	char *user = "root";
	char *password = "root";
	char *database = "lol";//要访问的数据库名称
    char query[200]="insert into member (id, name, age, ATK) values (7,'timo', 27, 86)";
	int queryResult;

	conn = mysql_init(NULL);//初始化MYSQL变量,mysql_init（&mysql）;
	//调用mysql_real_connect函数连接Mysql数据库, 连接失败时该函数返回0
	//MYSQL * STDCALL mysql_real_connect（
	//MYSQL *mysql， 前面定义的MYSQL变量
	//const char *host，host–MYSQL服务器的地址
	// const char *user   user–登录用户名；
	//const char *passwd，	passwd–登录密码
	//const char *db，	db–要连接的数据库
	//unsigned int port，	port–MYSQL服务器的TCP服务端口
	// const char *unix_socket，	unix_socket–unix连接方式,为NULL时表示不使用socket或管道机制
	//unsigned long clientflag）;	clientflag–Mysql运行为ODBC数据库的标记，一般取0
	if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		printf("Error connecting to database: %s\n", mysql_error(conn));
	}
    else
    {
		printf("Connected...\n");
	}

	//调用mysql_real_query函数进行数据库查询
	//参数说明：mysql–前面定义的MYSQL变量；q–SQL查询语句；length–查询语句的长度。
	//查询成功则该函数返回0。
	//int STDCALL mysql_real_query（MYSQL *mysql， const char *q， unsigned long length）;
	
	queryResult = mysql_query(conn, query);// 0 表示成功, 错误返回非零
	if(queryResult)
	{
		printf("Error making query:%s\n", mysql_error(conn));
	}
    else
    {
		printf("insert success\n");
	}

	/*通过调用mysql_store_result或mysql_use_result函数返回的MYSQL_RES变量获取查询结果数据。
	两个函数的原型分别为：
	MYSQL_RES * STDCALL mysql_store_result（MYSQL *mysql）;
	MYSQL_RES * STDCALL mysql_use_result（MYSQL *mysql）;
	这两个函数分别代表了获取查询结果的两种方式。第一种，调用mysql_store_result函数
	将从Mysql服务器查询的所有数据都存储到客户端，
	然后读取；第二种，调用mysql_use_result初始化检索，以便于后面一行一行的读取结果集，
	而它本身并没有从服务器读取任何数据，这种方式较之第一种速度更快且所需内存更少，
	但它会绑定服务器，阻止其他线程更新任何表，而且必须重复执行mysql_fetch_row读取数据，
	直至返回NULL，否则未读取的行会在下一次查询时作为结果的一部分返回，
	故经常我们使用mysql_store_result。*/

	/*调用mysql_fetch_row函数读取结果集数据。

	上述两种方式最后都是重复调用mysql_fetch_row函数读取数据。mysql_fetch_row函数的原型如下：

	MYSQL_ROW STDCALL mysql_fetch_row（MYSQL_RES *result）;
	参数result就是mysql_store_result或mysql_use_result的返回值。

	该函数返回MYSQL_ROW型的变量，即字符串数组，假设为row，则row［i］为第i个字段的值。当到结果集尾部时，此函数返回NULL。*/
	
	/*结果集用完后，调用mysql_free_result函数释放结果集，以防内存泄露。mysql_free_result函数的原型如下：

	void STDCALL mysql_free_result（MYSQL_RES *result）;*/

	//不再查询Mysql数据库时，调用mysql_close函数关闭数据库连接
	mysql_close(conn);

	return 0;
}
