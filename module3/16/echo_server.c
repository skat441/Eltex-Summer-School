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

#define MAX_CLIENT_COUNT 255
#define REGISTER_STR "REGISTER\0"
#define ACK_STR "ACK\0"
#define QUIT_STR "quit\0"
#define SERVER_PORT 51000
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
    header_for_send.source=htons(SERVER_PORT);
    header_for_send.dest=htons(source_port);
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
    new_data[data_size-1]=' ';
    new_data[data_size]=client_number+48;
    new_data[data_size+1]=0;
    //
    //client_counter[client_id]++;
    //Заполняем датаграмму
    char* new_datagramm=(char*)malloc(sizeof(char)*(data_size+2+UDP_HEADER_SIZE));
    memcpy(new_datagramm,&header_for_send,UDP_HEADER_SIZE);
    memcpy(new_datagramm+UDP_HEADER_SIZE,new_data,sizeof(char)*data_size+2);
    free(new_data);
    return new_datagramm;
}

int server_exit=0;

void safe_exit(int sig) {
    if(sig==SIGINT){server_exit=1;}
}

int main(){
    signal(SIGINT, safe_exit);
    int client_counter[MAX_CLIENT_COUNT];//Счетчик для каждого клиента
    int is_busy_port[MAX_CLIENT_COUNT];//Показатель того, занят ли порт под клиента
    for(int i=0;i<MAX_CLIENT_COUNT;i++){
        client_counter[i]=1;
    }
    int raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_sock == -1) {
        printf("Error to create socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // В Linux для raw socket не нужен SO_RCVALL
    // Просто привязываем к интерфейсу

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = SERVER_PORT;
    addr.sin_addr.s_addr = INADDR_ANY;
    struct sockaddr_in cliaddr;
    int clilen;
    
    if (bind(raw_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Failed to bind socket");
        close(raw_sock);
        exit(EXIT_FAILURE);
    }
    char buff[2048] = {0};

    do {
        int read_return = recvfrom(raw_sock, buff, sizeof(buff), FLAGS,(struct sockaddr *) &cliaddr,&clilen);
        if (read_return == -1) {
            printf("Error reading packet: %s\n", strerror(errno));
            continue;
        }
        
        if (read_return > 0) {
            struct iphdr *ip = (struct iphdr*)buff;
            struct udphdr *udp = (struct udphdr*)(buff + (ip->ihl * 4));
            
            // Получаем адреса
            uint32_t source_addr = ntohl(ip->saddr);
            uint32_t dest_addr = ntohl(ip->daddr);

            // Получаем порты
            uint16_t source_port = ntohs(udp->source);
            uint16_t dest_port = ntohs(udp->dest);
            
            // Вычисляем размер данных
            uint16_t ip_header_size = ip->ihl * 4;
            uint16_t udp_header_size = 8;
            uint16_t data_size = read_return - ip_header_size - udp_header_size;
            
            // Указатель на данные
            char *data = buff + ip_header_size + udp_header_size;
            //вывод сообщения в консоль
            printf("src: %u.%u.%u.%u:%d dest: %u.%u.%u.%u:%d ", 
                (source_addr >> 24) & 0xFF,
                (source_addr >> 16) & 0xFF,
                (source_addr >> 8) & 0xFF,
                source_addr & 0xFF,
                source_port,
                (dest_addr >> 24) & 0xFF,
                (dest_addr >> 16) & 0xFF,
                (dest_addr >> 8) & 0xFF,
                dest_addr & 0xFF,
                dest_port
                );
        
            printf("size: %d bytes ", data_size);
            
            if (data_size > 0) {
                printf("data: ");
                // Выводим данные как текст, если это возможно
                for (int i = 0; i < data_size && i < 100; i++) {
                    if (data[i] >= 32 && data[i] <= 126) {
                        printf("%c", data[i]);
                    } else {
                        printf("\\x%02x", (unsigned char)data[i]);
                    }
                }
            } else {
                printf("data: <empty>");
            }
            printf("\n");
            //Если сообщение адресовано серверу
            if(dest_port==51000){
                int client_id=source_port-SERVER_PORT;
                if(strcmp(data,REGISTER_STR)==0){//Регистрация нового клиента
                    if(is_busy_port[client_id]){//Порт занят другим клиентом
                        //Если отправим сообщение клиенту, оно продублируется на два клиента(существующий и новый) и оба с одинаковыми портами
                    }
                    else{
                        printf("register new client %d!\n",client_id);
                        is_busy_port[client_id]=1;
                        char* ack_datagramm=wrap_in_udp(ACK_STR,strlen(ACK_STR)+1,source_port);
                        sendto(raw_sock,ack_datagramm,strlen(ACK_STR)+1+UDP_HEADER_SIZE,FLAGS,(struct sockaddr *) &cliaddr,clilen);
                    }
                }
                else if(strcmp(data,QUIT_STR)==0){//Клиент завершает сессию
                    printf("Client %d disconnected\n",client_id);
                    is_busy_port[client_id]=0;
                    client_counter[client_id]=1;
                    char* ack_datagramm=wrap_in_udp(ACK_STR,strlen(ACK_STR)+1,source_port);
                    sendto(raw_sock,ack_datagramm,strlen(ACK_STR)+1+UDP_HEADER_SIZE,FLAGS,(struct sockaddr *) &cliaddr,clilen);
                }
                else{//Перенаправляем клиенту его же сообщение

                    char* new_datagramm=wrap_in_udp_with_adding_number(data,data_size,source_port,client_counter[client_id]);
                    client_counter[client_id]++;
                    printf("Redirect message to client\n");
                    sendto(raw_sock,new_datagramm,data_size+2+UDP_HEADER_SIZE,FLAGS,(struct sockaddr *) &cliaddr,clilen);
                    free(new_datagramm);

                }
            }
        }
    } while (!server_exit);

    close(raw_sock);
    return 0;
}