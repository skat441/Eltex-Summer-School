#include <stdlib.h>
void strToBin(const char* inputStr,char* outputStr){
    if (inputStr[0]=='r'||inputStr[0]=='-'){
        char str[11]="rwxrwxrwx\0";
        for(int i=0;i<10;i++){
            if(inputStr[i]==str[i]){
                outputStr[i]='1';
            }
            else{
                outputStr[i]='0';
            }
        }
        outputStr[9]='\0';
    }
    else{
        for(int i=0;i<3;i++){
            char tmp=inputStr[i];
            int num=atoi(&tmp);
            int k=0b100;
            for(int j=0;j<3;j++){
                if(num&k)outputStr[j+i*3]='1';
                else{outputStr[j+i*3]='0';}
                k=k>>1;
            }
        }
    }
}

void intToSymb(int mode,char* str_mode, char* bin_mode){
    char str[4];
    sprintf(str,"%o",mode&0777);
    strToBin(str,bin_mode);
    for(int i=0;i<3;i++){
        if(bin_mode[i*3]=='1')str_mode[i*3]='r';
        else str_mode[i*3]='-';
        if(bin_mode[i*3+1]=='1')str_mode[i*3+1]='w';
        else str_mode[i*3+1]='-';
        if(bin_mode[i*3+2]=='1')str_mode[i*3+2]='x';
        else str_mode[i*3+2]='-';
    }
}

int binToInt(char* bin_mode){
    int mode=0;
    for(int i=0;i<10;i++){
        if(bin_mode[i]=='1'){mode+=1;mode=mode<<1;}
        else{mode=mode<<1;}
    }
    mode=mode>>2;
    return mode;
}