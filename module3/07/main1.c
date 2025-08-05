#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#define SIZE 100
struct msgbuf {
    long mtype;
    char mtext[SIZE];
};

int
main(int argc, char *argv[])
{
    mqd_t  mqdes1;
    mqd_t  mqdes2;
    unsigned int prio=2;
    char buff[SIZE]={0};
    char nulbuf[SIZE]={0};
    char checkbuff[SIZE]={0};
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = SIZE;
    attr.mq_flags = 0;
    mqdes1 = mq_open("/my_queue1", O_RDWR);
    mqdes2 = mq_open("/my_queue2", O_RDWR);
    if (mqdes1 == -1 || mqdes2 == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    do{
        strcpy(buff,nulbuf);//clear buffer
        strcpy(checkbuff,nulbuf);//clear buffer

        printf("Enter your message:");
        fgets(buff,SIZE,stdin);
        buff[SIZE]=0;
        fseek(stdin,0,SEEK_END);

        sscanf(buff,"%s",checkbuff);//check input for "exit"
        if(strcmp(checkbuff,"exit")==0){
            if(mq_send(mqdes2,buff,strlen(buff),255)==-1){
                perror("mq_send");
                exit(EXIT_FAILURE);
            }
            
            break;
        }
        else{
            if(mq_send(mqdes2,buff,strlen(buff),1)==-1){
                perror("mq_send");
                exit(EXIT_FAILURE);
            }
        }

        if(mq_receive(mqdes1, buff, SIZE, &prio)==-1){
            perror("msgrecv");
            exit(EXIT_FAILURE);
        }
        if(prio==255)break;//check input for exit message
        printf("get:%s",buff);
    }while(1);
    exit(EXIT_SUCCESS);
}