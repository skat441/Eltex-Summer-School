#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#define SIZE 100
struct msgbuf {
    long mtype;
    char mtext[SIZE];
};

static void
send_msg(int client_id, int qid, int msgtype, char buff[SIZE])
{
    struct msgbuf  msg;
    msg.mtype = msgtype;
    strcpy(msg.mtext,buff);
    //mark msg source
    for(int i=SIZE;i>2;i--){//shift msg to add source id
        msg.mtext[i]=msg.mtext[i-3];
    }
    if(client_id<10){
        msg.mtext[0]='0';
        msg.mtext[1]=client_id+'0';
        msg.mtext[2]=' ';
    }
    else{
        msg.mtext[0]=client_id/10+'0';
        msg.mtext[1]=client_id%10+'0';
        msg.mtext[2]=' ';
    }
    msg.mtext[SIZE]=0;
    //printf("msg:%s\n",msg.mtext);
    if (msgsnd(qid, &msg, sizeof(msg.mtext),0) == -1)
    {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }
}

int
main(int argc, char *argv[])
{
    if(argc!= 2){
        printf("Usage: client.exe <client id>\n");
        exit(EXIT_FAILURE);
    }
    int client_id=atoi(argv[1]);
    if(client_id==10){
        printf("Server id is reserved\n");
        exit(EXIT_FAILURE);
    }
    int  qid, opt;
    int dest_id=0;
    int  msgkey = 2223;
    char answer=0;
    char buff[SIZE]={0};
    char nulbuf[SIZE]={0};
    char checkbuff[SIZE]={0};
    qid = msgget(msgkey, 0666);
    if (qid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    //register part
    strcpy(buff,"0 register\0");
    send_msg(client_id,qid,10,buff);
    printf("msg is send\n");
    struct msgbuf register_msg;
    msgrcv(qid, &register_msg, sizeof(register_msg.mtext), client_id,0);//get answer from server
    if(strcmp(register_msg.mtext,"FAIL")==0){
        printf("Client with such id already exist\n");
        exit(EXIT_FAILURE);
    }

    do{
        strcpy(buff,nulbuf);//clear buffer
        strcpy(checkbuff,nulbuf);//clear buffer
        printf("Enter your message:");
        fgets(buff,SIZE,stdin);
        fseek(stdin,0,SEEK_END);
        sscanf(buff,"%d %s",&dest_id, checkbuff);
        if(strcmp(checkbuff,"shutdown")==0){
            //send_msg(qid,1,buff);
            //send_msg(qid,255,buff);
            send_msg(client_id,qid,10,buff);
            break;
        }
        else{
            if(strcmp(buff,"\n")!=0)send_msg(client_id,qid,10,buff);
            //send_msg(qid,1,buff);
        }
        struct msgbuf msg;

        // if(msgrcv(qid, &msg, sizeof(msg.mtext), 2,0)==-1){
        //     perror("msgrecv");
        //     exit(EXIT_FAILURE);
        // }
        while(msgrcv(qid, &msg, sizeof(msg.mtext), client_id,IPC_NOWAIT) != -1){//check input for new messages
            printf("get new message:%s\n",msg.mtext);
        }
    }while(1);
    exit(EXIT_SUCCESS);
}