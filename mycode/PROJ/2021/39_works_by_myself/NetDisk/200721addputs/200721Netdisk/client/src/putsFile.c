#include "../include/head.h"
#include "../include/orders.h"
#include "../include/md5.h"

int putsFile(int sfd, char *filename){
    int ret = 0;
    Order_t order;
    memset(&order, 0, sizeof(order));
    //客户端打开文件, 如果打开失败, 说明命令错误, 目标文件不存在, 发送filesize = 0;
    int fd = open("filename", O_RDONLY);
    if(-1 == fd){
        order.orderNum = -1;
        strcpy(order.orderTarget, filename);
        send(sfd, &order, sizeof(order), 0);
        return -1;
    }
    //如果puts的filesize存在,则开始发送
    //1) get filesize
    struct stat fileStat;
    bzero(&fileStat, sizeof(fileStat));
    fstat(fd, &fileStat);
    //2) get file md5sum 
    char md5Ret[MD5_SIZE] = { 0 };
    ret = Compute_file_md5((const char*)filename, md5Ret); 
    ERROR_CHECK(ret, -1, "Compute_file_md5");
    
    /* //方案一:  把大小放入order.orderTarget中 */
    /* memset(&order, 0, sizeof(order)); */
    /* order.orderNum = 1; */
    /* strcpy(order.orderTarget, (char*)fileStat.st_size);/*or:  strncpy(order.orderTarget, (char*)fileStat.st_size, 8); *1/ */
    /* strcpy(order.orderTarget + sizeof(fileStat.st_size), md5Ret);// */

    //方案二: 
    memset(&order, 0, sizeof(order));
    order.orderNum = fileStat.st_size;
    strcpy(order.orderTarget, md5Ret);
    
    send(sfd, &order, sizeof(order), 0);

    //接收一下,文件是否能秒传成功!
    memset(&order, 0, sizeof(order));
    recv(sfd, &order, sizeof(order),0);
    if(1 == order.orderNum){
        transFile(fd, sfd, fileStat.st_size); 
    }else{
        printf("%s", order.orderTarget);
    }
    return 0;
}
