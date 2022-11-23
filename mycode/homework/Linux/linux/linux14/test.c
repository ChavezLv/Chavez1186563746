#include<func.h>
int main(){
    printf("helloworld\n");
    return 0;
}
//pthread_t ntid;
//void printids(const char* msg){
//    pid_t pid=getpid();
//    pthread_t tid=pthread_self();
//    printf("%s:pid%d,tid%lu,tid%lx\n",
//           msg,
//           pid,
//           tid,
//           tid);}
//void *start_routine(void* arg){
//        printids("new thread");
//        return NULL;
//    }
//int main(void){
//    int err=pthread_create(&ntid,NULL,start_routine,NULL);
//    printids("main thread");
//    sleep(1);
//    return 0;
//}
