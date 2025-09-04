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

int main(){
    int raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_sock == -1) {
        printf("Error to create socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // В Linux для raw socket не нужен SO_RCVALL
    // Просто привязываем к интерфейсу

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(raw_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Failed to bind socket");
        close(raw_sock);
        exit(EXIT_FAILURE);
    }

    char buff[2048] = {0};
    int read_return;
    int output_file = open("snifer.bin", O_CREAT|O_WRONLY|O_TRUNC, 0666);
    if (output_file == -1) {
        perror("Failed to open file");
        close(raw_sock);
        exit(EXIT_FAILURE);
    }

    printf("Starting UDP packet capture...\n");
    
    do {
        read_return = recv(raw_sock, buff, sizeof(buff), 0);
        if (read_return == -1) {
            printf("Error reading packet: %s\n", strerror(errno));
            continue;
        }
        
        if (read_return > 0) {
            struct iphdr *ip = (struct iphdr*)buff;
            printf("ip header size:%d ip version:%d\n",ip->ihl,ip->version);
            struct udphdr *udp = (struct udphdr*)(buff + (ip->ihl * 4));
            
            // Получаем адреса
            uint32_t source_addr = ntohl(ip->saddr);
            uint32_t dest_addr = ntohl(ip->daddr);

            // Получаем порты
            uint16_t source_port = ntohs(udp->source);
            uint16_t dest_port = ntohs(udp->dest);
            
            // Вычисляем размер данных
            uint16_t ip_header_size = ip->ihl * 4;
            printf("IP heade size:%d\n",ip_header_size);
            uint16_t udp_header_size = 8;
            uint16_t data_size = read_return - ip_header_size - udp_header_size;
            
            // Указатель на данные
            char *data = buff + ip_header_size + udp_header_size;
            
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
            
            // Записываем данные в файл
            write(output_file, buff, read_return);
        }
    } while (1);

    close(raw_sock);
    close(output_file);
    return 0;
}