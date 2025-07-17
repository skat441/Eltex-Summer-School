#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

uint32_t ipToInt(const char *ip){
    uint32_t val=0;
    uint8_t octet;
    char *ip_copy=strdup(ip);
    char *token=strtok(ip_copy,".");
    for(int i=0;i<4;++i){
        if(token!=NULL){
            octet=(uint8_t)atoi(token);
            val=(val<<8)|octet;
            token=strtok(NULL,".");
        }
    }
    free(ip_copy);
    return val;
}
int is_in_subnet(uint32_t ip, uint32_t mask, uint32_t subnet){
    if((ip&mask)==(subnet&mask))return 1;
    return 0;
}
int main(int argc, char* argv[]){
    uint32_t subnet=ipToInt(argv[1]);
    uint32_t mask = ipToInt(argv[2]);
    int N=atoi(argv[3]);
    int count=0;
    uint32_t newIP=0;
    srand(time(NULL)); 
    for(int i=0;i<N;i++){
        newIP=0;
        int r=192;
        newIP=newIP | r;
        newIP=newIP<<8;
        r=168;
        newIP=newIP | r;
        newIP=newIP<<8;
        r=rand()%3;
        newIP=newIP | r;
        newIP=newIP<<8;
        r=rand()%256;
        newIP=newIP | r;
        if(is_in_subnet(newIP,mask,subnet)){count+=1;}//printf("packet:%llu\n",newIP);
    }
    
    printf("All packets is:%d\nRight packets is:%d %f\n",N,count,((double)count) /N );
}