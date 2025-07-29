#include<stdio.h>
#include<unistd.h>
#include <string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define SIZE 256
char** getArgv(char buff[SIZE]){
    char argc[SIZE]={0};
    char buff2[SIZE]={0};
    char nulbuf[SIZE]={0};
    char** argv=NULL;
    
    sscanf(buff,"%s %s",buff2,argc);
    argv=(char**)calloc(sizeof(char*),(atoi(argc)+2));
    argv[atoi(argc)+1]=NULL;
    char* argbuf=strtok(buff," ");//=./
    argbuf=strtok(NULL," ");//=argc
    char *buff3;
    for (int i = 1; i < atoi(argc)+1; i++)
    {
        strcpy(buff2,nulbuf);
        buff3=strtok(NULL," ");
        argbuf=(char*)calloc(sizeof(char),strlen(buff3)+1);
        strcpy(argbuf,buff3);//
        argbuf[strlen(buff3)]=0;
        argv[i]=argbuf;
    }
    return argv;
}

int main(){
    char buff[SIZE]={0};
    char buff2[SIZE]={0};
    char nulbuf[SIZE]={0};
    do{
        strcpy(buff,nulbuf);
        strcpy(buff2,nulbuf);
        printf(">>");
        fgets(buff,SIZE,stdin);
        sscanf(buff,"%s",buff2);
        char argc[SIZE]={0};
        pid_t p = fork();
        if(p<0){
            perror("fork fail");
            exit(EXIT_FAILURE);
        }
        else if(p == 0){//child with
            char** argv=getArgv(buff);
            argv[0]=buff2;
            execvp(argv[0],argv);
            exit(EXIT_SUCCESS);
        }
        else{//Parent
            wait(NULL);
        }
    }while(strcmp(buff2,"exit")!=0);
    return 0;
}