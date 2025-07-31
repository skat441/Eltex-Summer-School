#include<stdio.h>
#include<stdlib.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    int max=INT_MIN;
    //printf("Number of args is:%d (",argc-1);
    for(int i=1;i<argc;i++){
        //printf("%s ",argv[i]);
        if(atoi(argv[i])>max)max=atoi(argv[i]);
    }
    printf("Max is:%d\n",max);
    return 0;
}