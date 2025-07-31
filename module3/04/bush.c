#include<stdio.h>
#include <err.h>
#include<unistd.h>
#include <string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<fcntl.h> 
#define SIZE 256
char** getArgv(char buff[SIZE]){
    if(buff==NULL)return NULL;
    char buff_copy[SIZE]={0};
    strcpy(buff_copy,buff);
    char nulbuf[SIZE]={0};
    char** argv=NULL;
    //printf("string is:%s\n",buff);
    int arg_count=0;//calculate count of args
    for(char* i=buff;*i!=0;i++)if(*i==' ')arg_count++;
    //printf("argc is:%d\n",arg_count);
    argv=(char**)calloc(sizeof(char*),(arg_count+2));
    argv[arg_count+1]=NULL;
    char* space=buff_copy;
    for(int i=0;i<arg_count;i++){//repalce all spaces by \0
        space=strchr(space,' ');
        if(space!=NULL)*space=0;
        space++;
    }
    //printf("first str:%s\n",buff_copy);
    char* argbuf;
    char *i_str=buff_copy;
    for(int i=0;i<arg_count+1;i++){
        argv[i]=(char*)calloc(sizeof(char),strlen(i_str)+1);
        strcpy(argv[i],i_str);
        argv[i][strlen(i_str)]=0;
        i_str=strchr(i_str,'\0')+1;
    }
    //printf("argv is send\n");
    return argv;
}

void free_argv(char** argv){
    if(argv==NULL)return;
    for(int i=0;argv[i]!=NULL;i++){
        free(argv[i]);
    }
}

int main(){
    char buff[SIZE]={0};
    char buff_copy[SIZE]={0};
    char buff2[SIZE]={0};
    char nulbuf[SIZE]={0};
    do{
        //clean buffers
        strcpy(buff,nulbuf);
        strcpy(buff_copy,nulbuf);
        strcpy(buff2,nulbuf);
        //get args
        printf(">>");
        fgets(buff,SIZE,stdin);
        buff[SIZE]=0;
        for(int i=0;i<strlen(buff);i++)if(buff[i]=='\n')buff[i]=0;//delete all \n
        
        sscanf(buff,"%s",buff2);
        if(strcmp(buff2,"exit")==0)break;
        
        if(buff[0]!=0){

            pid_t p = fork();
            if(p<0){
                perror("fork fail");
                exit(EXIT_FAILURE);
            }
            else if(p == 0){//child with
                strcpy(buff_copy,buff);//calculate number of pipes and commands
                int command_count=0;
                for(char* i=strtok(buff_copy,"|");i!=NULL;i=strtok(NULL,"|"))command_count++;
                int** pipes=(int**)calloc(sizeof(int*),(command_count-1));
                for(int i=0;i<command_count-1;i++){
                    pipes[i]=(int*)calloc(sizeof(int),(2));
                    if(pipe(pipes[i])==-1)err(EXIT_FAILURE, "pipe");
                }

                strcpy(buff_copy,buff);
                char* i_operation;
                //printf("command count:%d\n",command_count);
               // printf("buff copy is:%s\n",buff_copy);
                for(int i=0;i<command_count;i++){
                    //printf("buff copy in for is:%s\n",buff_copy);
                    if(i==0){
                        i_operation=strtok(buff_copy, "|");
                    }
                    else {
                        i_operation=strtok(NULL, "|");
                    }
                    //printf("i operation:%s\n",i_operation);
                    char** argv=getArgv(i_operation);
                    pid_t p2 = fork();
                    if(p2<0){
                        perror("fork fail");
                        exit(EXIT_FAILURE);
                    }
                    else if(p2 == 0){//child
                        if(command_count==1){//only one command
                            //printf("only one command in conveyor(%s)\n",argv[0]);
                            if(execvp(argv[0],argv)==-1)printf("Wrong command\n");
                        }
                        if(i==command_count-1 ){//last command in conveyor, read from pipe and write in stdin
                            //printf("last command in conveyor(%s)\n",argv[0]);
                            close(pipes[i-1][1]);
                            dup2(pipes[i-1][0],0);
                            close(pipes[i-1][0]);
                            //printf("argv:%s %d\n", argv[0],argv[1]);
                            if(execvp(argv[0],argv)==-1)printf("Wrong command\n");
                        }
                        if(i==0){//first command read from stdin and write in pipe
                            //printf("first command in conveyor(%s)\n",argv[0]);
                            close(pipes[i][0]);
                            dup2(pipes[i][1],1);
                            close(pipes[i][1]);
                            if(execvp(argv[0],argv)==-1)printf("Wrong command\n");
                        }
                        else{//midle command, read from pipe and write to pipe
                            //printf("middle command in conveyor(%s)\n",argv[0]);
                            close(pipes[i-1][1]);
                            close(pipes[i][0]);
                            dup2(pipes[i-1][0],0);
                            dup2(pipes[i][1],1);
                            close(pipes[i-1][0]);
                            close(pipes[i][1]);
                            if(execvp(argv[0],argv)==-1)printf("Wrong command\n");
                        }
                    }
                    else{//parent
                        if(i > 0) {
                            close(pipes[i-1][0]);
                            close(pipes[i-1][1]);
                        }
                        if(i < command_count-1) {
                            close(pipes[i][1]); // Close write end in parent
                        }
                        free_argv(argv);
                        free(argv);
                        wait(NULL);
                    }
                }
                exit(EXIT_SUCCESS);
            }
            else{//Parent
                wait(NULL);
            }
        }
    }while(1);
    return 0;
}