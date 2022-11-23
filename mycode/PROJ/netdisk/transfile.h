#include"func.h"
#include"mysql/mysql.h"
#include"oders.h"
#define BUFFSIZE 4096

int Sendfile(int clientfd,  int filesize, char *md5, int offset,char* filename);
int recvfile(int clientfd, int filesize, char *md5, int offset, char *filename);

typedef struct
{
    int length;
    char data[BUFFSIZE];
} train_t;

int sendCircle(int sfd, const char *buff, int length)
{
    const char *pbuf = buff;
    int ret = 0;
    int left = length;
    while (left > 0)
    {
        ret = send(sfd, pbuf, left, 0);
        if (ret < 0)
        {
            perror("send");
            return -1;
        }
        pbuf += ret;
        left -= ret;
    }
    return length - left;
}