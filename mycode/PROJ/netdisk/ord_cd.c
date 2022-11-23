#include"orders.h"
/*客户端发送命令号和参数,服务端解析参数,索引到指定文件,然后更改用户的当前目录(用户表的pwd);
返回客户的信息有错误信息(查无此路径) ;*/

//需要用户id
int order_cd(Ord_Info ord_info,int clientfd,reord_info_t retInfo,int userid){ 
    MYSQL* conn;
    mysql_connet(conn);
    int dirId = getAddr(ord_info);//利用getAddr解析路径
   //目录不存在 
    if(dirId == -1){
        retInfo.ordNum=-1;
        strcpy(retInfo.retInfo, "not this path");
        return -1;
    } 
    char query[200] = {0};
    sprintf(query, "update USER_INFO set pwd = %d where userID = %d",
     dirId,userid);
    //把pwd更改为指定路径的id号
    int ret = 0;
    ret = mysql_update(query);
    if(ret == -1){
        retInfo->ordNum = -1;
        strcpy(retInfo.retinfo, "fail");
        return -1;
    }
    strcpy(retInfo.retinfo, "success");    
    return dirId;
    mysql_close(conn);
}