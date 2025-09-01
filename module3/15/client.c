#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#define FILE_TRANSPORT 5

void error(const char *msg) {
perror(msg);
exit(0);
}

int main(int argc, char *argv[])
{
    int my_sock, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buff[1024];
    printf("TCP DEMO CLIENT\n");
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n",
        argv[0]);
        exit(0);
    }
    // извлечение порта
    portno = atoi(argv[2]);
    // Шаг 1 - создание сокета
    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0) error("ERROR opening socket");
    // извлечение хоста
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    // заполнение структуры serv_addr
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
    server->h_length);
    // установка порта
    serv_addr.sin_port = htons(portno);
    // Шаг 2 - установка соединения
    if (connect(my_sock, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR connecting");
    // Шаг 3 - чтение и передача сообщений
    if ((n = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) > 0)
    {
        // ставим завершающий ноль в конце строки
        buff[n] = 0;
        // выводим на экран
        if(strcmp(buff,"TOOMANYCLIENTS\0")==0){
            close(my_sock);
            printf("Too many clients");
            return 0;
        }
        else if(strcmp(buff,"ACCEPTED\0")==0){
            printf("S=>C:Enter option:\r\n1. sum\r\n2. neg\r\n3. mult\r\n4. div\r\n5. file transfer\r\n");
        }
        // читаем пользовательский ввод с клавиатуры
        printf("S<=C:");
        fgets(&buff[0], sizeof(buff) - 1, stdin);
        // проверка на "quit"
        if (!strcmp(&buff[0], "quit\n")) {
            // Корректный выход
            printf("Exit...");
            close(my_sock);
            return 0;
        }
        int answer=atoi(buff);
        // передаем строку клиента серверу
        send(my_sock, &answer, sizeof(int), 0);
        if( answer == FILE_TRANSPORT){//Нужно подготовить файл для передачи
            printf("Getting file...");
            char file_name_buff[1024];
            sscanf(buff,"%d %s",&n,file_name_buff);
            printf("%s\n",file_name_buff);
            send(my_sock,file_name_buff,sizeof(file_name_buff),0);//Досылаем имя файла

            recv(my_sock, &buff[0], sizeof(buff) - 1, 0);
            if(strcmp(buff,"NOTEXIST\0")==0){
                printf("File doesn't exist!");
                close(my_sock);
                return 0;
            }
            int fd = open(file_name_buff,O_CREAT | O_WRONLY, 0666);
            if(fd==-1){
                perror("open");
                exit(EXIT_FAILURE);
            }   
            int read_return;
            do {
                read_return = recv(my_sock, buff, sizeof(buff),0);
                if (read_return == -1) {
                    perror("read");
                    exit(EXIT_FAILURE);
                }
                if (write(fd, buff, read_return) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            } while (read_return > 0);
            close(fd);
            close(my_sock);
            return 0;
        }
        else{//Была выбрана мат. операция
            n=recv(my_sock, &buff[0], sizeof(buff) - 1, 0);//Получаем инструкции к первому аргументу
            buff[n] = 0;
            printf("S=>C:%s\n",buff);
            printf("S<=C:");
            fgets(&buff[0], sizeof(buff) - 1, stdin);
            answer=atoi(buff);//Первый аргумент
            send(my_sock, &answer, sizeof(int), 0);

            n=recv(my_sock, &buff[0], sizeof(buff) - 1, 0);//Получаем инструкции ко второму аргументу
            buff[n] = 0;
            printf("S=>C:%s\n",buff);
            printf("S<=C:");
            fgets(&buff[0], sizeof(buff) - 1, stdin);
            answer=atoi(buff);//второй аргумент
            send(my_sock, &answer, sizeof(int), 0);

            recv(my_sock,&answer,sizeof(int),0);//Результат от сервера
            printf("S=>C:%d\n",answer);

            close(my_sock);
            return 0;
        }
    }
    printf("Recv error\n");
    close(my_sock);
    return -1;
}