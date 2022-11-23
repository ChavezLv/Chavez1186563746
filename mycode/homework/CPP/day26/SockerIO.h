#ifndef __SockerIO_H__
#define __SockerIO_H__

class SockerIO{
public:
    explicit SockerIO(int fd);
    ~SockerIO();
    int readn(char *pstr ,int len);
    int writen(const char* pstr,int len);
    int readLine(char* pstr,int len);
private:
    int _fd;
};


#endif
