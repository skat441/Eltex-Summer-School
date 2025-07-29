#include<stdio.h>
#include<stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int sumlen=0;
    //printf("Number of args is:%d (",argc-1);
    for(int i=1;i<argc;i++){
        //printf("%s ",argv[i]);
        sumlen+=strlen(argv[i]);
    }
    //printf(")\n");
    char* newStr=(char*)calloc(sizeof(char),sumlen+1);
    int k=0;
    for(int i=1;i<argc;i++){
        for(int j=0;j<strlen(argv[i]);j++){
            newStr[k]=argv[i][j];
            k++;
        }
    }
    newStr[sumlen]='\0';
    printf("%s",newStr);
    return 0;
}