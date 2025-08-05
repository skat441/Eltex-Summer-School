#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    if(argc!= 2){
        printf("Usage: main.exe <number of strings>\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    sem_t* file_sem=sem_open("read_write",O_CREAT, O_RDWR, 1);
    int N=atoi(argv[1]);
    pid_t p = fork();
    if(p<0){
        perror("fork fail");
        exit(EXIT_FAILURE);
    }
    else if(p == 0){//child
        int num=1;
        int max_num=-1;
        int min_num=__INT_MAX__;
        sleep(1);
        int file=open("file.txt",O_RDWR,0777);
        for(int i=0;i<N;i++){//for N strings
            max_num=-1;
            min_num=__INT_MAX__;
            sem_wait(file_sem);
            do{
                read(file,&num,sizeof(int));
                if(num>max_num)max_num=num;
                if(num<min_num && num!=0)min_num=num;
            }while(num);
            //read(file,&num,sizeof(int));
            sem_post(file_sem);
            printf("max:%d min:%d\n",max_num,min_num);
        }
        close(file);
    }
    else{//parent
        int file=open("file.txt",O_CREAT|O_RDWR|O_TRUNC,0777);
        int num;
        for(int i=0;i<N;i++){
            sem_wait(file_sem);
            for(int j=0;j<rand()%10+1;j++){
                num=rand()%256+1;
                write(file,&num,sizeof(int));
            }
            num=0;
            write(file,&num,sizeof(int));
            sem_post(file_sem);
        }
        close(file);
    }
    sem_close(file_sem);
    sem_unlink("read_write");
    return 0;
}