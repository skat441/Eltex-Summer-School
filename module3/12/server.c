#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#define SIZE 256

static int shutdown_server=0;

void listener(int sig) {
    if(sig==2 || sig==3)shutdown_server=1;
}

int main(int argc, char *argv[])
{
    //signal(SIGINT, listener);
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(51000);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //printf("Server address:%d.%d.%d.%d\n", ntohl(local_addr.sin_addr.s_addr)>>24&0b11111111,(ntohl(local_addr.sin_addr.s_addr)>>16)&0b11111111,(ntohl(local_addr.sin_addr.s_addr)>>8)&0b11111111,ntohl(local_addr.sin_addr.s_addr)&0b11111111);
    if(bind(udp_socket, (struct sockaddr *) &local_addr, sizeof(local_addr))<0){
        perror("socket bind");
        close(udp_socket);
        exit(EXIT_FAILURE);
    }
    int clients[SIZE]={0};
    int  qid, opt, dest_prio=0,source_prio=0;
    int  msgkey = 2223;
    char answer=0;
    char buff[SIZE]={0};
    char nulbuf[SIZE]={0};
    char checkbuff[SIZE]={0};

    struct sockaddr_in source_addr;
    socklen_t len;

    do{
        dest_prio=0;

        if(recvfrom(udp_socket,&buff,SIZE,0,(struct sockaddr *) &source_addr,&len)==-1){
            perror("socket recv");
            close(udp_socket);
            exit(EXIT_FAILURE);
        }
        
        printf("get:%s (from:%d.%d.%d.%d)\n\n",buff, ntohl(source_addr.sin_addr.s_addr)>>24&0b11111111,(ntohl(source_addr.sin_addr.s_addr)>>16)&0b11111111,(ntohl(source_addr.sin_addr.s_addr)>>8)&0b11111111,ntohl(source_addr.sin_addr.s_addr)&0b11111111);
         //strcpy(buff,nulbuf);//clear buffer
         strcpy(checkbuff,nulbuf);//clear buffer
        sscanf(buff,"%d %s",&dest_prio, checkbuff);
        
        if(strcmp(checkbuff,"register")==0){//register new client
            if(clients[ntohl(source_addr.sin_addr.s_addr)&0b11111111]==1){//client already exist
                strcpy(buff,"FAIL\0");
                if(sendto(  udp_socket, buff, strlen(buff)+1, 0, (struct sockaddr *) &source_addr, len) < 0){
                    perror("send answer to login");
                    close(udp_socket);
                    exit(1);
                }
                            //send_msg(qid,source_prio,checkbuff);
            }
            else{
                clients[ntohl(source_addr.sin_addr.s_addr)&0b11111111]=1;
                strcpy(buff,"SUCCESS\0");
                if(sendto(  udp_socket, buff, strlen(buff)+1, 0, (struct sockaddr *) &source_addr, len) < 0){
                    perror("send answer to login");
                    close(udp_socket);
                    exit(1);
                }
                //send_msg(qid,source_prio,checkbuff);
            }
        }
        else if(strcmp(checkbuff,"shutdown")==0){//delete client
            if(dest_prio==10)shutdown_server=1;
            if(clients[ntohl(source_addr.sin_addr.s_addr)&0b11111111]==1){//client exists
                clients[ntohl(source_addr.sin_addr.s_addr)&0b11111111]=0;
                printf("shutdown client\n");
            }
            //break;
        }
        else{
            if(clients[dest_prio]==1){
                
                struct sockaddr_in dest_addr;
                dest_addr.sin_family = AF_INET;
                dest_addr.sin_port = htons(51001);
                char client_ip[11]={"127.0.0."};
                client_ip[8]=dest_prio/10+'0';
                client_ip[9]=dest_prio%10+'0';
                client_ip[10]=0;
                dest_addr.sin_addr.s_addr = inet_addr(client_ip);
                printf("send new message to:%s\n",client_ip);

                if (dest_addr.sin_addr.s_addr == INADDR_NONE) {
                    perror("Invalid IP address");
                    close(udp_socket);
                    exit(EXIT_FAILURE);
                }


                if(sendto(  udp_socket, buff, SIZE, 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr)) < 0){
                    perror("redirect answer");
                    close(udp_socket);
                    exit(EXIT_FAILURE);
                }
            }
            //if(clients[dest_prio]==1)send_msg(qid,dest_prio,msg.mtext);//redirect msg if client exists
        }
        if(shutdown_server){
            printf("shutdown server\n");
            //msgctl(qid, IPC_RMID, NULL);
            close(udp_socket);
            exit(EXIT_SUCCESS);
        }
    }while(1);
}