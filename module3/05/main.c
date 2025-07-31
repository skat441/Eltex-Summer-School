#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
static writting = 1;
static sigint_count=0;
static signal_is_catched=0;

void listener(int sig) {
    if(sig==2 || sig==3)signal_is_catched=1;
    if(sig==2)sigint_count++;
    if(sigint_count==3)writting=0;
}

void main() {
    int count=0;
    signal(SIGINT, listener);
    signal(SIGQUIT, listener);
    // signal(SIGINT, SIG_IGN);
    int file=open("save.txt",O_CREAT|O_RDWR|O_TRUNC,0777);
    if(file<0){
        printf("Error to open file");
        return;
    }
    do{
        if(signal_is_catched){write(file,"signal is caught!\0",strlen("signal is caught!\0"));signal_is_catched=0;}
        write(file,&count,sizeof(int));
        count++;
        sleep(1);
    }while(writting);
    close(file);
    getc(stdin);
}