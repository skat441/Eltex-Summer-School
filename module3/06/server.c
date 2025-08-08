#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#define SIZE 100

static int shutdown_server=0;

struct msgbuf {
    long mtype;
    char mtext[SIZE];
};

static void
send_msg(int qid, int msgtype, char buff[SIZE])
{
    struct msgbuf  msg;
    strcpy(msg.mtext,buff);
    msg.mtype = msgtype;

    if (msgsnd(qid, &msg, sizeof(msg.mtext),0) == -1)
    {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }
}

int
main(int argc, char *argv[])
{
    //signal(SIGINT, listener);
    int clients[SIZE]={0};
    int  qid, opt, dest_prio=0,source_prio=0;
    int  msgkey = 2223;
    char answer=0;
    char buff[SIZE]={0};
    char nulbuf[SIZE]={0};
    char checkbuff[SIZE]={0};
    qid = msgget(msgkey, IPC_CREAT | 0666);
    if (qid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    do{
        struct msgbuf msg;
        //printf("block for first message\n");
        if(msgrcv(qid, &msg, sizeof(msg.mtext), 10,0)==-1){//lock for new messages for server
            perror("msgrecv");
            exit(EXIT_FAILURE);
        }
        
        printf("get:%s\n",msg.mtext);
         strcpy(buff,nulbuf);//clear buffer
         strcpy(checkbuff,nulbuf);//clear buffer
        sscanf(msg.mtext,"%d %d %s",&source_prio,&dest_prio, buff);
        
        if(strcmp(buff,"register")==0){//register new client
            if(clients[source_prio]==1){//client already exist
                strcpy(checkbuff,"FAIL\0");
                send_msg(qid,source_prio,checkbuff);
            }
            else{
                clients[source_prio]=1;
                strcpy(checkbuff,"SUCCESS\0");
                send_msg(qid,source_prio,checkbuff);
            }
        }
        else if(strcmp(buff,"shutdown")==0){//delete client
            if(dest_prio==10)shutdown_server=1;
            if(clients[source_prio]==1){//client exists
                clients[source_prio]=0;
            }
            //break;
        }
        else{
            if(clients[dest_prio]==1)printf("send new message to:%d\n",dest_prio);
            if(clients[dest_prio]==1)send_msg(qid,dest_prio,msg.mtext);//redirect msg if client exists
        }
        if(shutdown_server){
            printf("shutdown server\n");
            msgctl(qid, IPC_RMID, NULL);
            exit(EXIT_SUCCESS);
        }
    }while(1);
}