#include"../include/orders.h"
int read_Addr(char *path, int userId) {
    char path_copy[PATH_LEN] = { 0 };
    char delim[2] = "/";
    char *token;
    
    int path_size = 0;
    
    
    char ret[512] = { 0 };  //数据库查询结果

    int userRootDirID = 0;  //用户家目录ID
    int searchRet = 0;  //数据库查询返回值，查询失败返回-1
    int pwdDirID = 0;  //用户当前工作目录

    MYSQL *conn = mysqlConnect();
    char comman[COMMAN_LEN] = { 0 };
    

    //查找用户家目录ID，存储在userRootDirID中
    sprintf(comman, 
            "select dirID from DIR_INFO where userID = %d and parentDirID = %d", userId, 0);
    searchRet = mysqlSearch(conn, comman, ret);        
    if(searchRet == -1){
        return -1;
    }
    userRootDirID = atoi(ret);

    //查找用户当前目录ID，存储在pwdDirID中
    bzero(comman, COMMAN_LEN);//将内存,块（字符串）的前n个字节清零<string.h>
    sprintf(comman, 
            "select pwdDirID from USER_INFO where userID = %d", userId);
    searchRet = mysqlSearch(conn, comman, ret);        
    if(searchRet == -1){
        return -1;
    }
    pwdDirID = atoi(ret);

    if(strlen(path) == 0){
        return pwdDirID;//返回用户当前工作目录
    }

    strcpy(path_copy, path);

    char eachPath[PATH_NUM][PATH_LEN];//二维数组20*512 ?
    memset(eachPath, 0, PATH_LEN * PATH_NUM);

   
    token = strtok(path_copy, delim);//以delim标识符分解字符串,此文件delim为/
    //表示根目录
    if(token == NULL){
        return userRootDirID;
    }


    strcpy(eachPath[0], token);
    ++path_size;//目录层级数
    //每一层级的目录名都存放到eachpath中
    while (token != NULL) {
        token = strtok(NULL, delim);
        if (!token) {
            break;
        }
        strcpy(eachPath[path_size], token);
        ++path_size;
    }

    int dirCount = path_size;//路径的层级数

    int *arr_dirID = (int*)(calloc(dirCount, sizeof(int)));   //动态整形数组保存每一个目录的ID

    for (int i = 0; i < path_size; ++i) {
        printf("%s\n", eachPath[i]);
    }



    //如果路径以"/"开头，从用户家目录开始查找
    if(path[0] == '/'){
        //  "/"后面为".."
        if(strcmp(eachPath[0], "..") == 0){
            return -1;
        }
        //  "/"后面为"."
        if(strcmp(eachPath[0], ".") == 0){
            arr_dirID[0] = pwdDirID;//pwdDirID初值为零
        }else {
            //  "/"后面为文件夹名，查找是否存在第一个目录并且其父目录ID为0的目录
            sprintf(comman, 
                "select dirID from DIR_INFO where userID = %d and dirName = '%s' and parentDirID = %d", 
                userId, eachPath[0], userRootDirID);
            searchRet = mysqlSearch(conn, comman, ret);        
            //将什么数据返回给ret?
            printf("error\n");
            if(searchRet == -1){
                return -1;
            }
            printf("after error\n");
            bzero(comman, COMMAN_LEN);//bzero,将内存块前n个字节清零;commandlen=512
            //comman是传递数据库指令字段
            arr_dirID[0] = atoi(ret);
        }


        //依次向下查找
        for(int i = 1; i < path_size; ++i){
            //遇到路径中有".."
            if(strcmp(eachPath[i], "..") == 0){
                //查找父目录ID
                bzero(comman, COMMAN_LEN);
                sprintf(comman, 
                        "select parentDirID from DIR_INFO where userID = %d and dirID = %d",
                        userId, arr_dirID[i-1]);
                searchRet = mysqlSearch(conn, comman, ret);        
                if(searchRet == -1){
                    return -1;
                }
                if(atoi(ret) == 0){
                    return userRootDirID;//相当于/dir/..,又返回root目录
                }
                arr_dirID[i] = atoi(ret);
                continue;
            }
            //遇到路径中有"."
            if(strcmp(eachPath[i], ".") == 0){
                arr_dirID[i] = arr_dirID[i-1];
                continue;
            }
            //
            bzero(comman, COMMAN_LEN);
            sprintf(comman, 
                    "select dirID from DIR_INFO where userID = %d and dirName = '%s' and parentDirID = %d", 
                    userId, eachPath[i], arr_dirID[i-1]);
            searchRet = mysqlSearch(conn, comman, ret);        
            if(searchRet == -1){
                return -1;
            }
            if(atoi(ret) == 0){
                return userRootDirID;
            }
            arr_dirID[i] = atoi(ret);
        }
    }

    //以"../"开头，从当前目录的父目录开始查找
    else if(strcmp(eachPath[0], "..") == 0){  
        bzero(comman, COMMAN_LEN);
        sprintf(comman, 
                "select parentDirID from DIR_INFO where userID = %d and dirID = %d", userId, pwdDirID);
        searchRet = mysqlSearch(conn, comman, ret);        
        if(searchRet == -1){
            return -1;
        }
        
        arr_dirID[0] = atoi(ret);


        //依次向下查找
        for(int i = 1; i < path_size; ++i){
            //遇到路径中有".."
            if(strcmp(eachPath[i], "..") == 0){
                //查找父目录ID
                bzero(comman, COMMAN_LEN);
                sprintf(comman, 
                        "select parentDirID from DIR_INFO where userID = %d and dirID = %d", userId, arr_dirID[i-1]);
                searchRet = mysqlSearch(conn, comman, ret);        
                if(searchRet == -1){
                    return -1;
                }
                if(atoi(ret) == 0){
                    return userRootDirID;
                }
                arr_dirID[i] = atoi(ret);
                continue;
            }
            //遇到路径中有"."
            if(strcmp(eachPath[i], ".") == 0){
                arr_dirID[i] = arr_dirID[i-1];
                continue;
            }
            
            bzero(comman, COMMAN_LEN);
            sprintf(comman, 
                    "select dirID from DIR_INFO where userID = %d and dirName = '%s' and parentDirID = %d", userId, eachPath[i], arr_dirID[i-1]);
            searchRet = mysqlSearch(conn, comman, ret);        
            if(searchRet == -1){
                return -1;
            }
            if(atoi(ret) == 0){
                return userRootDirID;
            }
            arr_dirID[i] = atoi(ret);
        }

    }

    //以目录名开头，或者以"./"开头，从当前目录开始查找
    else{
        //以"."开头
        if(strcmp(eachPath[0], ".") == 0){
            arr_dirID[0] = pwdDirID;
        }
        //以目录名开头
        else{
            //查找当前目录下是否有路径中第一个文件夹
            bzero(comman, COMMAN_LEN);
            sprintf(comman,
                    "select dirID from DIR_INFO where userID = %d and parentDirID = %d and dirName = '%s'", userId, pwdDirID, eachPath[0]);
            searchRet = mysqlSearch(conn, comman, ret);
            if(searchRet == -1){
                return -1;
            }
            arr_dirID[0] = atoi(ret);
        }
        

        //依次向下查找
        for(int i = 1; i < path_size; ++i){
            //遇到路径中有".."
            if(strcmp(eachPath[i], "..") == 0){
                //查找父目录ID
                bzero(comman, COMMAN_LEN);
                sprintf(comman, 
                        "select parentDirID from DIR_INFO where userID = %d and dirID = %d", userId, arr_dirID[i-1]);
                searchRet = mysqlSearch(conn, comman, ret);        
                if(searchRet == -1){
                    return -1;
                }
                if(atoi(ret) == 0){
                    return userRootDirID;
                }
                arr_dirID[i] = atoi(ret);
                continue;
            }
            //遇到路径中有"."
            if(strcmp(eachPath[i], ".") == 0){
                arr_dirID[i] = arr_dirID[i-1];
                continue;
            }

            bzero(comman, COMMAN_LEN);
            sprintf(comman, 
                    "select dirID from DIR_INFO where userID = %d and dirName = '%s' and parentDirID = %d", userId, eachPath[i], arr_dirID[i-1]);
            searchRet = mysqlSearch(conn, comman, ret);        
            if(searchRet == -1){
                return -1;
            }
            if(atoi(ret) == 0){
                return userRootDirID;
            }
            arr_dirID[i] = atoi(ret);
        }
    }

    mysqlClose(conn);
    
    for(int i = 0; i < path_size; ++i){
        printf("$mysql>>No.%d = %d\n", i + 1, arr_dirID[i]);
    }
    if(arr_dirID[path_size-1] == 0){
        return userRootDirID;
    }
    return arr_dirID[path_size-1];//最深层次的文件id
}
