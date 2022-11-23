#include "../include/head.h"

int main(int argc,char*argv[]){
    Daemon();
    while(1){
        time_t now;
        time(&now);
        struct tm *pTm = localtime(&now);
        syslog(LOG_INFO, "%4d%02d%02d %02d:%02d\n", pTm->tm_yday + 1900, pTm->tm_mon + 1, \
               pTm ->tm_mday, pTm->tm_hour, pTm->tm_min);
        sleep(2);
    }
    return 0;
}

