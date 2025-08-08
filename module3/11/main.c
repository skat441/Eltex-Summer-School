#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
static int stop_signal=0;
void listener(int sig) {
    if(sig==2)stop_signal=1;
}

int main(int argc, char *argv[]){
    if(argc!= 1){
        printf("Usage: ./main.exe\n");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, listener);
    srand(time(NULL)+getpid());
    int data_count=0;
    //Semaphors
    sem_unlink("max_min");
    sem_t* shm_max_min_sem=sem_open("max_min",O_CREAT, O_RDWR, 1);

    //shared memory
    int shm_d=shm_open("nums",O_CREAT|O_RDWR|O_TRUNC,0666);
    if(ftruncate(shm_d, sizeof(int) * 11) == -1) {
        perror("ftruncate nums failed");
        exit(EXIT_FAILURE);
    }
    int* shm_data=(int*)mmap(NULL,sizeof(int)*11,PROT_READ|PROT_WRITE,MAP_SHARED,shm_d,0);
    if(shm_data==MAP_FAILED){
        printf("Error to get shared memory\n");
        exit(EXIT_FAILURE);
    }
    int shm_mm=shm_open("max_min_shm",O_CREAT|O_RDWR|O_TRUNC,0666);
    if(ftruncate(shm_mm, sizeof(int) * 2) == -1) {
        perror("ftruncate nums failed");
        exit(EXIT_FAILURE);
    }
    int* shm_max_min=(int*)mmap(NULL,sizeof(int)*2,PROT_READ|PROT_WRITE,MAP_SHARED,shm_mm,0);
    if(shm_max_min==MAP_FAILED){
        printf("Error to get shared memory\n");
        exit(EXIT_FAILURE);
    }
    //fork
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
        for(int i=0;1;i++){//for N strings
            sleep(1);
            max_num=-1;
            min_num=__INT_MAX__;
            sem_wait(shm_max_min_sem);
            int j=0;
            do{
                num=shm_data[j];
                if(num>max_num)max_num=num;
                if(num<min_num && num!=0)min_num=num;
                j++;
            }while(num);
            shm_max_min[0]=max_num;
            shm_max_min[1]=min_num;
            sem_post(shm_max_min_sem);
            if(stop_signal)break;
        }
        exit(EXIT_SUCCESS);
    }
    else{//parent
        int num;
        for(int i=0;1;i++){
            sleep(1);
            sem_wait(shm_max_min_sem);
            if(i!=0)printf("[MAX_MIN] max:%d min:%d\n",shm_max_min[0],shm_max_min[1]);
            int max_count=rand()%10+1;
            for(int j=0;j<max_count;j++){
                data_count++;
                num=rand()%256+1;
                shm_data[j]=num;
            }
            shm_data[max_count]=0;
            sem_post(shm_max_min_sem);
            if(stop_signal)break;
        }
        wait(NULL);
        //close(file);
    }
    sem_unlink("max_min");
    sem_close(shm_max_min_sem);
    // shmdt(shm_data);
    // shmctl(shm_d, IPC_RMID, NULL);
    munmap(shm_data, sizeof(int) * 11);
    close(shm_d);
    shm_unlink("nums");
    munmap(shm_max_min, sizeof(int) * 2);
    close(shm_mm);
    shm_unlink("max_min_shm");
    printf("data count:%d\n",data_count);
    // shmdt(shm_max_min);
    // shmctl(shm_mm, IPC_RMID, NULL);
    return 0;
}