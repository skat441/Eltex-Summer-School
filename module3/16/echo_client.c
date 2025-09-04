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

#define SERVER_PORT 51000
#define REGISTER_STR "REGISTER\0"
#define EXIT_STR "EXIT\0"
#define FLAGS 0

// Структура для IP заголовка
struct iphdr {
    uint8_t version:4;
    uint8_t ihl:4;
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

int main(int argc, char *argv[]){
    
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
    servaddr.sin_port = htons(51000);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    struct udphdr datagramm;

    sendto(raw_sock,REGISTER_STR,strlen(REGISTER_STR),FLAGS,(struct sockaddr*)&servaddr,sizeof(servaddr));


    // do{

    // }while(1);

    return 0;
}