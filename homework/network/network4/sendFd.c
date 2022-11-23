#include<func.h>
/* struct msghdr {
  //第一组数据:套接口地址成员msg_name与msg_namelen(不关注)
               void         *msg_name;        //optional address 
               socklen_t     msg_namelen;     //size of address 
  //第二组数据:I/O向量引用msg_iov与msg_iovlen。     
               struct iovec *msg_iov;         //scatter/gather array 
               size_t        msg_iovlen;      //# elements in msg_iov 
  //第三组数据:附属数据缓冲区成员msg_control与msg_controllen。
               void         *msg_control;     //ancillary data, see below 
               size_t        msg_controllen;  //ancillary data buffer len 
  //第四组数据:接收信息标记位msg_flags.(不关注)
               int           msg_flags;       //flags (unused) 
           };*/


/* struct cmsghdr {
         socklen_t cmsg_len;
         int       cmsg_level;
         int       cmsg_type;
    //   u_char     cmsg_data[]; 
};
相关成员(宏)
cmsg_len     附属数据的字节计数，这包含结构头的尺寸。这个值是由CMSG_LEN()宏计算的。
cmsg_level   这个值表明了原始的协议级别(例如，SOL_SOCKET)。
cmsg_type    这个值表明了控制信息类型(例如，SCM_RIGHTS)。
cmsg_data    这个成员并不实际存在。他用来指明实际的额外附属数据所在的位置。
*/
int sendFd(int pipefd,int fd){
    //第二组数据的配置
    struct iovec iov;
    memset(&iov,0,sizeof(iov));
    char buff[10]={0};
    strcpy(buff,"hello");//为什么不直接赋值
    iov.iov_base=buff;
    iov.iov_len=strlen(buff);
    
    //第三组数据配置
    //POSIX规定,描述符(即fd)包含在msghdr结构体中,作为辅助数据发送;
    //具体包含在msg_control成员中
    size_t len=CMSG_LEN(sizeof(fd));//获得结构体大小
    struct cmsghdr *cmsg=(struct cmsghdr*)calloc(1,len);
    cmsg->cmsg_len=len;
    cmsg->cmsg_level=SOL_SOCKET;
    cmsg->cmsg_type=SCM_RIGHTS;

    //返回cmsg的首地址;指明额外附属数据的位置
    int *pfd=(int*)CMSG_DATA(cmsg);
    *pfd=fd;

    //定义msg
    struct msghdr msg;
    memset(&msg,0,sizeof(msg));
    msg.msg_iov=&iov;
    msg.msg_iovlen=1;
    msg.msg_control=cmsg;
    msg.msg_controllen=len;
    //完成以上配置即可使用sendmsg()来传递文件描述符;
    //
    int ret=sendmsg(pipefd,&msg,0);
    return 0;
}
