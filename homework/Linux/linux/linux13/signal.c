#include<func.h>

void func1(int signo){
    printf("interrupt\n");
}
void func2(int signo){
    printf("quit\n");
}
void func3(int signo){
    printf("stop\n");
}

int main(void){
    
    signal(SIGQUIT,func2);
    signal(SIGINT,func1);
    signal(SIGTSTP,func3);
    
    while(1);
}
