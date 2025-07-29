#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
int isNumber(char* arg){//0 - not number, 1 - integer, 2 - float
    for (char* i=arg;*i!=0;i++){
        if(!(*i=='-' || *i=='.' || (*i>='0' && *i<'9') ))return 0; 
    }
    int dot_count=0;
    int minus_count=0;
    for (char* i=arg;*i!=0;i++){
        if(*i=='.')dot_count++;
        if(*i=='-')minus_count++;
    }
    if(dot_count>1)return 0;
    if(minus_count>1)return 0;
    if(dot_count==1)return 2;
    return 1;
}

int main(int argc, char *argv[])
{
    int rv;
    int n;
    double f;
    pid_t p = fork();
    if(p<0){
      perror("fork fail");
      exit(EXIT_FAILURE);
    }
    else if (p == 0){//child
        for(int i=1;i<argc;i++){
            if(i%2==0){
                if(isNumber(argv[i])==1){sscanf(argv[i],"%d",&n);printf("%d ",n*2);}
                else if(isNumber(argv[i])==2){sscanf(argv[i],"%lf",&f);printf("%lf ",f*2);}
                else{
                    printf("%s ", argv[i]);
                }
            }
        }
        exit(EXIT_SUCCESS);
    }
    else{//parent
        for(int i=1;i<argc;i++){
            if(i%2==1){
                if(isNumber(argv[i])==1){sscanf(argv[i],"%d",&n);printf("%d ",n*2);}
                else if(isNumber(argv[i])==2){sscanf(argv[i],"%lf",&f);printf("%lf ",f*2);}
                else{
                    printf("%s ", argv[i]);
                }
            }
        }
        wait(&rv);
        printf("\n");
    }
    return 0;
}