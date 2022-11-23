#include"func.h"
#include"orders.h"

//传过来的命令,参数应当包含文件大小,md5码,文件类型,路径
int order_puts(Ord_Info ordinfo,int clientfd){
 //客户端发送的下载请求信息应该包含pwd+filename
    MYSQL* conn = mysqlConnect();
    
    //查询数据库
    char query[256];
    char result[128];
    int count;
    int ret;
    int pwdID;
    char type='0';
    char md5[128]="\0";
    char filename[128];
    int filesize;//int类型太小,可以改为long long
    int userid;//尚未获取
    char path[256];
    sscanf(ordinfo.args, "%s %s %d %c" , md5, path, &filesize, type);

    //获取可用空间大小
    struct statfs diskInfo;
	statfs("/files", &diskInfo);
	unsigned long long blocksize = diskInfo.f_bsize;//每个block里包含的字节数
    unsigned long long availableDisk = diskInfo.f_bavail * blocksize;
 
    if(filesize>availableDisk){
        printf("Error:The available space of the server is insufficient!\n");
        return -1;
    }
    char eachID[256];
    char** eachdir=getAddr(path,userid,&count,eachID);
    strcpy(filename,eachdir[count]);
    
    for(int i=1;i<count;i++){
        memset(query,'0',sizeof(query));
        sprintf(query, "select id from user_file where parent_id= %d and\
            fileName = '%s'", eachdir[i-1], eachdir[i]);
        ret = mysqlSearch(conn, query,result);
        pwdID=atoi(result);
        if(ret=-1){
            printf("no this path");
            //1,先检查指定路径的文件是否存在,yes取消上传,no进行第二步
            //2,先判断服务器是否有该文件,有则不传(整张文件表中检索md5)
        memset(query,'0',sizeof(query));
        memset(result,'0',sizeof(result));
        sprintf(query, "select id from user_file where md5= %s" , md5);
        ret = mysqlSearch(conn, query,result);

        if(-1 != result){
                //如果相等的md5值
                bzero(query, sizeof(query));
                //不用上传文件,直接再数据库中插入表项
                sprintf(query, "insert into user_file (id, parent_id, filename, owner_id,md5,filesize,type) values (%d, %d, %d, '%s')",
                pwd-1, pwd, filename, ownerid,md5,filesize,type);
                //用户表和文件表的关联键?
                mysqlInsert(conn, query);
                return 0;
                }else{
            int ret=recvfile(clientfd,sendfd,filesize,md5,offset,filename);
            //根据返回值判断错误
             break;}
        }       
    }    
    mysql_close(conn);
    return 0;
}
