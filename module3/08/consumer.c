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
    srand(time(NULL));
    int file=open(argv[1],O_RDWR,0777);
    if(file==-1){
        printf("No such file\n");
        exit(EXIT_FAILURE);
    }
    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock[2] = {{0, 0, 0},
                               {0, 1, 0}};
    int file_sem;
    read(file,&file_sem,sizeof(int)==0);
    int N=atoi(argv[2]);
    int num;
    sleep(1);
    for(int i=0;i<N;i++){
        int max_num=-1;
        int min_num=__INT_MAX__;
        semop(file_sem,&lock,1);
        do{
                read(file,&num,sizeof(int));
                if(num>max_num)max_num=num;
                if(num<min_num && num!=0)min_num=num;
            }while(num);
        num=0;
        printf("max:%d min:%d\n",max_num,min_num);
        semop(file_sem,&unlock,2);
        sleep(1);
    }
    close(file);
    // sem_close(file_sem);
    // sem_unlink("read_write");
    return 0;
}