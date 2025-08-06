#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>        /* For mode constants */
 #include <sys/sem.h>
 #include <sys/types.h>
 #include <sys/ipc.h>
#include <time.h>
#include <unistd.h>

union semun {
 int val;                  /* значение для SETVAL */
 struct semid_ds *buf;     /* буферы для  IPC_STAT, IPC_SET */
 unsigned short *array;    /* массивы для GETALL, SETALL */
                           /* часть, особенная для Linux: */
 struct seminfo *__buf;    /* буфер для IPC_INFO */
};


int main(int argc, char *argv[]){
    if(argc!= 3){
        printf("Usage: producer.exe <file> <number of strings>\n");
        exit(EXIT_FAILURE);
    }
    int file=open(argv[1],O_CREAT|O_RDWR,0777);
    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock[2] = {{0, 0, 0},
                               {0, 1, 0}};
    int file_sem;
    if(read(file,&file_sem,sizeof(int))==0){//file didn't exist earlier
        printf("file didn't exist earlier\n");
        file_sem=semget(argv[1],1,IPC_CREAT|O_RDWR);
        union semun arg;
        arg.val=1;
        semctl(file_sem, 0, SETVAL, arg);
        write(file,&file_sem,sizeof(int));
    }
    close(file);
    int N=atoi(argv[2]);
    int num;
    srand(time(NULL)+getpid());
    for(int i=0;i<N;i++){
        semop(file_sem,&lock,1);
        file=open(argv[1],O_APPEND|O_RDWR,0777);
        for(int j=0;j<5;j++){
            num=rand()%256+1;
            write(file,&num,sizeof(int));
        }
        num=0;
        write(file,&num,sizeof(int));
        close(file);
        semop(file_sem,&unlock,2);
        sleep(1);
    }
    close(file);
    // sem_close(file_sem);
    // sem_unlink("read_write");
    return 0;
}