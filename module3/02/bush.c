#include<stdio.h>
#include<unistd.h>
#include <string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define SIZE 256
char** getArgv(char buff[SIZE]){
    char buff_copy[SIZE]={0};
    strcpy(buff_copy,buff);
    char nulbuf[SIZE]={0};
    char** argv=NULL;
    
    int arg_count=-1;//except first argument
    for(char* i=strtok(buff," ");i!=NULL;i=strtok(NULL," "))arg_count++;

    argv=(char**)calloc(sizeof(char*),(arg_count+2));
    argv[arg_count+1]=NULL;

    char* argbuf=strtok(buff_copy," ");//read programm name
    argv[0]=(char*)calloc(sizeof(char),strlen(argbuf)+1);
    strcpy(argv[0],argbuf);
    argv[0][strlen(argbuf)]=0;

    char *buff3;

    for (int i = 1; i < arg_count+1; i++)
    {
        buff3=strtok(NULL," ");
        argbuf=(char*)calloc(sizeof(char),strlen(buff3)+1);
        strcpy(argbuf,buff3);//
        argbuf[strlen(buff3)]=0;
        argv[i]=argbuf;
    }
    return argv;
}

void free_argv(char** argv){
    for(int i=0;argv[i]!=NULL;i++){
        free(argv[i]);
    }
}

int main(){
    char buff[SIZE]={0};
    char buff2[SIZE]={0};
    char nulbuf[SIZE]={0};
    do{
        //clean buffers
        strcpy(buff,nulbuf);
        strcpy(buff2,nulbuf);
        //get args
        printf(">>");
        fgets(buff,SIZE,stdin);
        buff[SIZE]=0;
        for(int i=0;i<strlen(buff);i++)if(buff[i]=='\n')buff[i]=0;//delete all \n
        if(buff[0]!=0){
            char** argv=getArgv(buff);//get str with args for execvp

            pid_t p = fork();
            if(p<0){
                perror("fork fail");
                exit(EXIT_FAILURE);
            }
            else if(p == 0){//child with
                execvp(argv[0],argv);
                exit(EXIT_SUCCESS);
            }
            else{//Parent
                wait(NULL);
                free_argv(argv);
                free(argv);
            }
        }
        sscanf(buff,"%s",buff2);
    }while(strcmp(buff2,"exit")!=0);
    return 0;
}