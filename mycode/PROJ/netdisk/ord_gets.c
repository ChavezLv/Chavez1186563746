#include"orders.h"

#include"transfile.h"
/*需要参数形如args=/chavezlv/files/hello.txt*/
int ord_gets(Ord_Info ordinfo,int clientfd)
{   int userid,offset=0;//尚未获取获取
    //客户端发送的下载请求信息应该包含pwd+filename
    MYSQL* conn = mysqlConnect();
    //查询数据库
    char query[256];
    char result[128];
    int count;//记录目录层级
    int ret;
    int eachID[PATH_MAX];
    //虚拟当前位置
    char** eachdir=getAddr(ordinfo.args,userid,&count,eachID);
    //判断getAddr的返回值;
    //确定文件存在开始执行发送操作
    //确定文件大小
    memset(query,'0',sizeof(query));
    sprintf(query,"select filesize from user_file where id=%d \
            and userid=%d",//ownerid外键链接到userid
            eachID[count],userid);
    mysqlSearch(conn, query,result);
    int filesize=atoi(result);
    //获取md5码
    memset(query,'0',sizeof(query));
    sprintf(query,"select md5 from user_file where id=%d and userid=%d",eachID[count],userid);
    mysqlSearch(conn, query,result);
    char* md5=result;
    //获取文件偏移量
    memset(query,'0',sizeof(query));
    sprintf(query,"select offset from user_file where id=%d and userid=%d",eachID[count],userid);
    mysqlSearch(conn, query,result);
    offset=atoi(result);
    ret =Sendfile(clientfd,filesize,md5,offset,eachdir[count]);//client和offset的获取?
    //用ret判断返回值
    mysql_close(conn);
    return 0;
}
