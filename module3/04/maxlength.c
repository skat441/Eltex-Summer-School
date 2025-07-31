#include<stdio.h>
#include<stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int maxIndex=1;
   // printf("Number of args is:%d (",argc-1);
    for(int i=1;i<argc;i++){
        //printf("%s ",argv[i]);
        if(strlen(argv[i])>strlen(argv[maxIndex]))maxIndex=i;
    }
    printf("Maxlength is:%s\n",argv[maxIndex]);
    return 0;
}