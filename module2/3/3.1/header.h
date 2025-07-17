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
    if(mode<010){
        sprintf(str,"%o",mode&0777);//get str like "7", but must be "007"
        str[2]=str[0];
        str[0]='0';
        str[1]='0';
    }
    else if(mode<100){
        sprintf(str,"%o",mode&0777);//get str like "70", but must be "070"
        str[2]=str[1];
        str[1]=str[0];
        str[0]='0';
    }
    else{
        sprintf(str,"%o",mode&0777);
    }
    printf("new=%s\n",str);
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

int rwx_to_bin(char* rwx){
    if(!strcmp(rwx,"rwx"))     return 0b111;
    else if(!strcmp(rwx,"rw")) return 0b110;
    else if(!strcmp(rwx,"wx")) return 0b011;
    else if(!strcmp(rwx,"rx")) return 0b101;
    else if(!strcmp(rwx,"r"))  return 0b100;
    else if(!strcmp(rwx,"w"))  return 0b010;
    else if(!strcmp(rwx,"x"))  return 0b001;
    else return 0;
}

void update_filestat(struct stat* filestat, char* flags){
    //filestat->st_mode=filestat->st_mode|0b111111111;
    if(flags[1]=='+' || flags[2]=='+' || flags[3]=='+'){//get new privilages
        int newStat=0b000000000;
        char* who=strtok(flags,"+");
        char* priv=strtok(NULL,"+");
        int flag=0;
        if(strlen(who)==1){
            switch (who[0])
            {
            case 'u':
                flag=rwx_to_bin(priv);
                newStat=flag<<6;
                filestat->st_mode=filestat->st_mode|newStat;
                return;
                break;
            case 'g':
                flag=rwx_to_bin(priv);
                newStat=flag<<3;
                filestat->st_mode=filestat->st_mode|newStat;
                return;
                break;
            case 'o':
                flag=rwx_to_bin(priv);
                newStat=flag;
                filestat->st_mode=filestat->st_mode|newStat;
                return;
                break;
            default:
                break;
            }
        }
        else if(strlen(who)==2){
            if(!strcmp(who,"ug")){
                flag=rwx_to_bin(priv);
                newStat=flag<<3;
                newStat+=flag;
                newStat=newStat<<3;
                filestat->st_mode=filestat->st_mode|newStat;
                 return;
            }
            else if(!strcmp(who,"go")){
                flag=rwx_to_bin(priv);
                newStat=flag<<3;
                newStat+=flag;
                filestat->st_mode=filestat->st_mode|newStat;
                return;
            }
            else if(!strcmp(who,"uo")){
                flag=rwx_to_bin(priv);
                newStat=flag<<3;
                newStat=newStat<<3;
                newStat+=flag;
                filestat->st_mode=filestat->st_mode|newStat;
                return;
            }
        }
        else if(strlen(who)==3){//ugo
            flag=rwx_to_bin(priv);
            newStat=flag<<3;
            newStat+=flag;
            newStat=newStat<<3;
            newStat+=flag;
            filestat->st_mode=filestat->st_mode|newStat;
            return;
        }
    }
    else if(flags[1]=='-' || flags[2]=='-' || flags[3]=='-'){//revoke privilages
        int newStat=0b000000000;
        char* who=strtok(flags,"-");
        char* priv=strtok(NULL,"-");
        int flag=0;
        if(strlen(who)==1){
            switch (who[0])
            {
            case 'u':
                flag=rwx_to_bin(priv);
                newStat=flag<<6;
                newStat=~newStat;
                filestat->st_mode=filestat->st_mode&newStat;
                return;
                break;
            case 'g':
                flag=rwx_to_bin(priv);
                newStat=flag<<3;
                newStat=~newStat;
                filestat->st_mode=filestat->st_mode&newStat;
                return;
                break;
            case 'o':
                flag=rwx_to_bin(priv);
                newStat=flag;
                newStat=~newStat;
                filestat->st_mode=filestat->st_mode&newStat;
                return;
                break;
            default:
                break;
            }
        }
        else if(strlen(who)==2){
            if(!strcmp(who,"ug")){
                flag=rwx_to_bin(priv);
                newStat=flag<<3;
                newStat+=flag;
                newStat=newStat<<3;
                newStat=~newStat;
                filestat->st_mode=filestat->st_mode&newStat;
                 return;
            }
            else if(!strcmp(who,"go")){
                flag=rwx_to_bin(priv);
                newStat=flag<<3;
                newStat+=flag;
                newStat=~newStat;
                filestat->st_mode=filestat->st_mode&newStat;
                return;
            }
            else if(!strcmp(who,"uo")){
                flag=rwx_to_bin(priv);
                newStat=flag<<3;
                newStat=newStat<<3;
                newStat+=flag;
                newStat=~newStat;
                filestat->st_mode=filestat->st_mode&newStat;
                return;
            }
        }
        else if(strlen(who)==3){//ugo
            flag=rwx_to_bin(priv);
            newStat=flag<<3;
            newStat+=flag;
            newStat=newStat<<3;
            newStat+=flag;
            newStat=~newStat;
            filestat->st_mode=filestat->st_mode&newStat;
            return;
        }
    }
}