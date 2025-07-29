#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    int sum=0;
    //printf("Number of args is:%d (",argc-1);
    for(int i=1;i<argc;i++){
       // printf("%s ",argv[i]);
        sum+=atoi(argv[i]);
    }
    printf("Sum is:%d\n",sum);
    return 0;
}