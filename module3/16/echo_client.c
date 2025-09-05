#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>

#define SERVER_PORT 51000
#define REGISTER_STR "REGISTER\0"
#define EXIT_STR "EXIT\0"
#define ACK_STR "ACK\0"
#define QUIT_STR "quit\0"
#define FLAGS 0
#define UDP_HEADER_SIZE 8

// Структура для IP заголовка
struct iphdr {
    uint8_t ihl:4;
    uint8_t version:4;
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
};

// Структура для UDP заголовка
struct udphdr {
    uint16_t source;
    uint16_t dest;
    uint16_t len;
    uint16_t check;
};

char* wrap_in_udp(char* buff, int data_size,uint16_t source_port){
    struct udphdr header_for_send;
    //Заполняем udp заголовок
    header_for_send.source=htons(source_port);
    header_for_send.dest=htons(SERVER_PORT);
    header_for_send.len=data_size+UDP_HEADER_SIZE;
    header_for_send.check=0;
    //
    //client_counter[client_id]++;
    //Заполняем датаграмму
    char* new_datagramm=(char*)malloc(sizeof(char)*(data_size+UDP_HEADER_SIZE));
    memcpy(new_datagramm,&header_for_send,UDP_HEADER_SIZE);
    memcpy(new_datagramm+UDP_HEADER_SIZE,buff,sizeof(char)*data_size);

    return new_datagramm;
}

char* wrap_in_udp_with_adding_number(char* buff, int data_size,uint16_t source_port,int client_number){
    struct udphdr header_for_send;
    //Заполняем udp заголовок
    header_for_send.source=htons(SERVER_PORT);
    header_for_send.dest=htons(source_port);
    header_for_send.len=data_size+2+UDP_HEADER_SIZE;
    header_for_send.check=0;
    //Добавляем циферку в конец
    char* new_data=(char*)malloc(sizeof(char)*data_size+2);
    strcpy(new_data,buff);
    new_data[data_size]=' ';
    new_data[data_size+1]=client_number+48;
    //
    //client_counter[client_id]++;
    //Заполняем датаграмму
    char* new_datagramm=(char*)malloc(sizeof(char)*(data_size+2+UDP_HEADER_SIZE));
    memcpy(new_datagramm,&header_for_send,UDP_HEADER_SIZE);
    memcpy(new_datagramm+UDP_HEADER_SIZE,new_data,sizeof(char)*data_size+2);
    free(new_data);
    return new_datagramm;
}

int is_connected=1;

void safe_exit(int sig) {
    if(sig==SIGINT){is_connected=0;}
}


int main(int argc, char *argv[]){
    signal(SIGINT, safe_exit);
    if(argc!=2){
        printf("Usage ./echo_client.exe <Port>");
        exit(EXIT_FAILURE);
    }
    int client_port=atoi(argv[1]);
    printf("Start client on port:%d\n",client_port);
    fflush(0);
    int raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_sock == -1) {
        printf("Error to create socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // В Linux для raw socket не нужен SO_RCVALL
    // Просто привязываем к интерфейсу

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = client_port;
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(raw_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Failed to bind socket");
        close(raw_sock);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    struct udphdr header_for_send;
    //Заполняем udp заголовок
    header_for_send.source=htons(client_port);
    header_for_send.dest=htons(SERVER_PORT);
    header_for_send.len=UDP_HEADER_SIZE+strlen(REGISTER_STR);
    header_for_send.check=0;
    int servlen;
    char* datagramm=(char*)malloc(sizeof(char)*(UDP_HEADER_SIZE+strlen(REGISTER_STR)+1));
    memcpy(datagramm,&header_for_send,UDP_HEADER_SIZE);
    memcpy(datagramm+UDP_HEADER_SIZE,REGISTER_STR,sizeof(char)*strlen(REGISTER_STR)+1);

    sendto(raw_sock,datagramm,UDP_HEADER_SIZE+strlen(REGISTER_STR)+1,FLAGS,(struct sockaddr*)&servaddr,sizeof(servaddr));
    
    free(datagramm);

    char msg_buff[1024];
    char console_buff[256];
    struct iphdr* ip;
    struct udphdr *udp;
    int read_return;
    do{//Получаем сообщения пока не получим нужное от сервера(Номер порта назначения совпадает с нашим)
        read_return=recvfrom(raw_sock,msg_buff,sizeof(msg_buff),FLAGS,(struct sockaddr*)&servaddr,&servlen);
        ip = (struct iphdr*)msg_buff;
        udp = (struct udphdr*)(msg_buff + (ip->ihl * 4));
    }while(ntohs(udp->dest)!=client_port);

    // Вычисляем размер данных
    uint16_t ip_header_size = ip->ihl * 4;
    uint16_t data_size = read_return - ip_header_size - UDP_HEADER_SIZE;
    // Указатель на данные
    char *data = msg_buff + ip_header_size + UDP_HEADER_SIZE;
    if(strcmp(data,ACK_STR)!=0){//Не получили одобрение от сервера на подключение
        close(raw_sock);
        return 0;
    }
    printf("Your logined!\n");
    do{
        //читаем ввод с клавиатуры
        printf("Server<=You:");
        fgets(&console_buff[0], sizeof(console_buff) - 1, stdin);
        console_buff[256]=0;
        for(int i=0;i<256;i++)if(console_buff[i]=='\n')console_buff[i]=0;
        if(strcmp(console_buff,QUIT_STR)==0){
            is_connected=0;
        }
        if(!is_connected){
            strcpy(console_buff,QUIT_STR);
        }
        //Отправляем сообщение на сервер
        char* new_datagramm=wrap_in_udp(console_buff,strlen(console_buff),client_port);
        sendto(raw_sock,new_datagramm,UDP_HEADER_SIZE+strlen(console_buff)+1,FLAGS,(struct sockaddr*)&servaddr,sizeof(servaddr));
        free(new_datagramm);

        do{//Получаем сообщения пока не получим нужное от сервера(Номер порта назначения совпадает с нашим)
            read_return=recvfrom(raw_sock,msg_buff,sizeof(msg_buff),FLAGS,(struct sockaddr*)&servaddr,&servlen);
            ip = (struct iphdr*)msg_buff;
            udp = (struct udphdr*)(msg_buff + (ip->ihl * 4));
        }while(ntohs(udp->dest)!=client_port);

        // Вычисляем размер данных
        uint16_t ip_header_size = ip->ihl * 4;
        uint16_t data_size = read_return - ip_header_size - UDP_HEADER_SIZE;
        // Указатель на данные
        char *data = msg_buff + ip_header_size + UDP_HEADER_SIZE;
        printf("Server=>You:%s\n",data);
        
    }while(is_connected);
    close(raw_sock);
    return 0;
}