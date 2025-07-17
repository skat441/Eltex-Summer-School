#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include "header.h"

int main(int argc, char* argv[]){
    if (argc==2){
    
        if(strlen(argv[1])==3 || strlen(argv[1])==9){//it not file:P
            char resStr[10]={0};
            strToBin(argv[1],resStr);
            printf("%s\n",resStr);
        }
        else{//this is file!
            char* filename=argv[1];
            struct stat buf;
            stat(filename,&buf);
            printf("%o\n",buf.st_mode&0777); 
            char str_mode[10]={0};
            char bin_mode[10]={0};
            intToSymb(buf.st_mode&0777,str_mode,bin_mode);
            printf("%s\n%s\n",str_mode,bin_mode); 
        }
    }
    else if(argc==3){
        //old printf
        if(strlen(argv[1])==3){//int view
            int mode;
            sscanf(argv[1],"%o",&mode);
            char str_mode[10]={0};
            char bin_mode[10]={0};
            intToSymb(mode,str_mode,bin_mode);
            printf("old:%o\nold:%s\nold:%s\n",mode,str_mode,bin_mode);
        }
        else if(strlen(argv[1])==9 && (argv[1][0]=='r'||argv[1][0]=='-')){//symb view
            char bin_mode[10];
            strToBin(argv[1],bin_mode);
            int mode=binToInt(bin_mode);
            printf("old:%o\nold:%s\nold:%s\n",mode,argv[1],bin_mode);
        }
        else{//this is file!
            char* filename=argv[1];
            struct stat buf;
            stat(filename,&buf);
            printf("old:%o\n",buf.st_mode&0777); 
            char str_mode[10]={0};
            char bin_mode[10]={0};
            intToSymb(buf.st_mode&0777,str_mode,bin_mode);
            printf("old:%s\nold:%s\n",str_mode,bin_mode); 
        }
        //new printf
        if(strlen(argv[2])==3){//int view
            int mode;
            sscanf(argv[2],"%o",&mode);
            char str_mode[10]={0};
            char bin_mode[10]={0};
            intToSymb(mode,str_mode,bin_mode);
            printf("new:%o\nnew:%s\nnew:%s\n",mode,str_mode,bin_mode);
        }
        else if(strlen(argv[2])==9 && (argv[2][0]=='r'||argv[2][0]=='-')){//symb or bin view
            char bin_mode[10];
            strToBin(argv[2],bin_mode);
            int mode=binToInt(bin_mode);
            printf("new:%o\nnew:%s\nnew:%s\n",mode,argv[2],bin_mode);
        }
    }
    return 0;
}