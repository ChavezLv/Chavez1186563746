#include<func.h>
#include"string.h"
#include<mysql/mysql.h>
#define USERMAX 100
#define SALT_LEN 15
#define RANDOM_STR_LEN 8
#define PATH_LEN 512
#define PATH_NUM 20
#define DIR_NAME_LEN 20
#define COMMAN_LEN 512
enum orderNum{
    CD=3,GETS,PUTS
};//命令可拓展

typedef struct ord_info_s{
    int ordNum;
    char args[256];
}Ord_Info;

typedef struct reord_info_s{
    int ordNum;
    char retinfo[256];//存放返回信息
}reord_info_t;
/*
char** analysis_path (Ord_Info ordinfo,int *count){
    char buff[256];
    const char delim='/';
    buff=ordinfo.args;
    char* each_dir[256];
    char token[128];
    count=0;//路径层级
    while(token=strtok(buff,delim)){
        each_dir[&count++]=token;
    }
    return each_dir;
}*/

int order_cd(Ord_Info ord_info,int clientfd,reord_info_t retInfo);
int ord_gets(Ord_Info ordinfo,int clientfd);
int order_puts(Ord_Info ordinfo,int clientfd);

//作用:传入路径,返回解析后的路径字符串数组;analysis_path的改进版
void* getAddr(char* path,int userId,int* hierarchy,int eachID[PATH_NUM]){
    char delim[2] = "/";    //定义strtok的分隔符
    char *token;            //存储每个层级的目录名
    *hierarchy = 0;          //目录层级
    
    char ret[512] = { 0 };  //数据库查询结果

    int userhome = 0;       //用户家目录ID
    int searchRet = 0;      //数据库查询返回值，查询失败返回-1
    int pwd = 0;            //用户当前工作目录

    MYSQL* conn = mysqlConnect();
    char comman[COMMAN_LEN] = { 0 };
    

    //查找用户家目录ID，存储在userhome中
    sprintf(comman, 
            "select id from USER_FILE where userID = %d \
            and parentDirID = %d", userId, 0);
    searchRet = mysqlSearch(conn, comman, ret);        
    if(searchRet == -1){
        return (void *)-1;
    }
    userhome = atoi(ret);//返回用户根目录

    //查找用户当前目录ID，存储在pwd中

    bzero(comman, COMMAN_LEN);
    //将内存,块（字符串）的前n个字节清零<string.h>
    sprintf(comman, 
            "select pwd from USER_ID where userID = %d", userId);//用户表中取pwd
    searchRet = mysqlSearch(conn, comman, ret);        
    if(searchRet == -1){
        return (void*)-1;
    }
    //获取pwd
    pwd= atoi(ret);
    
    //获得home和pwd之后继续解析
    if(strlen(path)==1&&strcmp(path,"/")==0){
        char* retStr="you didn`t have root privilege\n";
        return retStr;//返回
    }
    //还有..和.的情况,暂时不考虑

    //开始分情况解析路径
    char *eachPath[PATH_NUM];//存放每个层级文件名

    memset(eachPath, 0, PATH_LEN);
   
    token = strtok(path, delim);// 以/解析路径
    strcpy(eachPath[0], token);
    ++*hierarchy;//目录层级数
    //根目录cd  和cd ~
    if(token == NULL||strcmp(token,"/")){
        char result[128]={0};
        sprintf(comman,"select username from USER_ID where id =%d",userhome);
        mysqlSearch(conn,comman,result);
        eachID[0]=userhome;
        return result;//home目录
    }else{

    //每一层级的目录名都存放到eachpath中
    while (token != NULL) {
        token = strtok(NULL, delim);
        if (!token) {
            break;
        }
        //查询虚拟文件表,根据查询结果判断是否存在
        bzero(comman, COMMAN_LEN);
        sprintf(comman,
                    "select id from userfile where userID = %d \
                    and parentID = %d and dirName = '%s'",
                     userId, pwd, eachPath[0]);
                     //where条件子句包括用户表和文件表的字段,联合查询,两表应当有链接关系
        searchRet = mysqlSearch(conn, comman, ret);
        if(searchRet == -1){
                //查询失败返回负1
                printf("no this path\n");
                return (void*)-1;
            }
            //查询成功,将目录或文件名存入
        eachID[*hierarchy]=atoi(ret);
        strcpy(eachPath[*hierarchy], token);
        ++*hierarchy;
        }
    }
    
    //路径的总层级数analysis_path,需要用到再传出
    return eachPath;
}

