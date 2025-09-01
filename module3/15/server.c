#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <poll.h>

#define CLIENT_STATUS_DEFAULT      -1
#define CLIENT_STATUS_ACCPTED       0
#define CLIENT_STATUS_SET_OPERATION 1
#define CLIENT_STATUS_GET_FIRST_ARG 2
#define TIMEOUT 1000
#define INVALID_FD -1
#define MAX_CLIENT_COUNT 2
#define OPERATION_DEF -1
#define OPERATION_ADD 1
#define OPERATION_NEG 2
#define OPERATION_MLT 3
#define OPERATION_DIV 4
#define OPERATION_TRANSPORT 5
#define file_not_exist "NOTEXIST\0"
#define file_exist "EXIST\0"

// функция обслуживания
// подключившихся пользователей
void domath(int);
// функция обработки ошибок
void error(const char *msg) {
    perror(msg);
    exit(1);
}
// количество активных пользователей
static int nclients = 0;

// печать количества активных
// пользователей
void printusers(int client_count) {
    if(client_count) {
    printf("%d user on-line\n",
    client_count);
    }
    else {
    printf("No User on line\n");
    }
}
// функция обработки данных
int myfunc(int a, int b, int operation) {
         if(operation==OPERATION_ADD)return a + b;
    else if(operation==OPERATION_NEG)return a - b;
    else if(operation==OPERATION_MLT)return a * b;
    else if(operation==OPERATION_DIV)return a / b;
    else return a+b;
}

static int shutdown_server=0;

void listener(int sig) {
    if(sig==SIGINT)shutdown_server=1;
}

// void sigchld_handler(int sig) {
//     int status;
//     pid_t pid;
    
//     // Ожидаем завершения дочернего процесса
//     while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
//         nclients--; // Уменьшаем счетчик при завершении дочернего процесса
//         printf("-disconnect (PID: %d)\n", pid);
//         printusers();
//     }
// }

void set_nonblocking(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) error("fcntl F_GETFL");
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) error("fcntl F_SETFL");
}

int main(int argc, char *argv[])
{
    signal(SIGINT, listener);
    //signal(SIGCHLD, sigchld_handler);
    char buff[1024]; // Буфер для различных нужд
    int sockfd, newsockfd; // дескрипторы сокетов
    int portno; // номер порта
    int pid; // id номер потока
    socklen_t clilen; // размер адреса клиента типа socklen_t
    struct sockaddr_in serv_addr, cli_addr; // структура сокета сервера и клиента

    int clients_statuses[MAX_CLIENT_COUNT+1];
    struct pollfd sockets[MAX_CLIENT_COUNT+1];
    int number_of_clients=0;
    int first_numbers[MAX_CLIENT_COUNT+1]={0};
    int operation[MAX_CLIENT_COUNT+1]={OPERATION_DEF};
    
    for (int i = 0; i <= MAX_CLIENT_COUNT+1; i++) {
        sockets[i].fd = INVALID_FD;
        sockets[i].events = 0;
        clients_statuses[i]=-1;
        first_numbers[i]=0;
    }

    printf("TCP SERVER DEMO\n");
    // ошибка в случае если мы не указали порт
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    // Шаг 1 - создание сокета
    sockets[0].fd = socket(AF_INET, SOCK_STREAM, 0);//0 socket is server passive socket
    if (sockets[0].fd < 0) error("ERROR opening socket");
    // Шаг 2 - связывание сокета с локальным адресом
    set_nonblocking(sockets[0].fd);
    bzero((char*) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // сервер принимает подключения на все IP-адреса
    serv_addr.sin_port = htons(portno);
    if (bind(sockets[0].fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    sockets[0].events = POLLIN;
    // Шаг 3 - ожидание подключений, размер очереди - 5
    listen(sockets[0].fd, 5);
    clilen = sizeof(cli_addr);
    int ret=0;
    // Шаг 4 - извлекаем сообщение из очереди (цикл извлечения запросов на подключение)
    while (!shutdown_server)
    {
        ret = poll(sockets,MAX_CLIENT_COUNT+1,TIMEOUT);
        if (ret < 0) error("ERROR in poll");

        if(sockets[0].revents & POLLIN){//get new connection
            newsockfd = accept(sockets[0].fd, (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0) {
                if (shutdown_server) break;
                error("ERROR on accept");
            }
            if(number_of_clients==MAX_CLIENT_COUNT){//all slots are occupied
                write(newsockfd, "TOOMANYCLIENTS\0",sizeof("TOOMANYCLIENTS\0"));
                close(newsockfd);
                continue;
            }
            number_of_clients++;
            printusers(number_of_clients);
            write(newsockfd, "ACCEPTED\0",sizeof("ACCEPTED\0"));
            //Привязать к другому сокету активное подключение
            //Найти свободный слот
            int slot=1;
            for(slot;slot<=MAX_CLIENT_COUNT;slot++){
                if(sockets[slot].fd==INVALID_FD)break;
            }
            set_nonblocking(newsockfd);
            sockets[slot].fd=newsockfd;
            sockets[slot].events=POLLIN|POLLHUP|POLLERR;
            clients_statuses[slot]=CLIENT_STATUS_ACCPTED;
            printf("New client connected (fd: %d)\n", newsockfd);
            // // вывод сведений о клиенте
            // struct hostent *hst;
            // hst = gethostbyaddr((char *)&cli_addr.sin_addr, 4, AF_INET);
            // printf("+%s [%s] new connect!\n",
            // (hst) ? hst->h_name : "Unknown host",
            // (char*)inet_ntoa(cli_addr.sin_addr));
            // nclients++;
            // printusers();
            // pid = fork();
            // if (pid < 0) error("ERROR on fork");
            // if (pid == 0) {
            //     close(sockfd);
            //     domath(newsockfd);
            //     exit(0);
            // }
            // else close(newsockfd);
        }
        for(int i=1;i<=MAX_CLIENT_COUNT;i++){
            if (sockets[i].fd == INVALID_FD) continue;

            if (sockets[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {//Клиент отключился по ошибке, завис или сокет закрыт
                printf("Client (socket: %d) disconnected\n", sockets[i].fd);
                clients_statuses[i]=-1;
                first_numbers[i]=-1;
                close(sockets[i].fd);
                sockets[i].fd = INVALID_FD;
                continue;
            }

            if(sockets[i].revents & POLLIN){//обработка события клиента
                if(clients_statuses[i]==CLIENT_STATUS_ACCPTED){//клиент только принят, клиент отправляет код операции(или и название файла)
                    
                    read(sockets[i].fd,&operation[i],sizeof(int));
                    if(operation[i]==OPERATION_TRANSPORT){//Особый случай, запрос файла
                        sleep(1);//между отправкой номера операции и имени файла может возникнуть небольшой интервал времени
                        char file_name[1024];
                        int n=read(sockets[i].fd,file_name,sizeof(file_name));
                        if(n==-1){
                            printf("Read error\n");
                        }
                        printf("open file:%s\n",file_name);
                        FILE* fd=fopen(file_name,"r");
                        if(fd==NULL){//файл не существует
                            printf("Error to open file\n");
                            write(sockets[i].fd,file_not_exist,sizeof(file_not_exist));
                            //Сброс соединения
                            clients_statuses[i]=CLIENT_STATUS_DEFAULT;
                            first_numbers[i]=0;
                            printf("Client (socket: %d) disconnected\n", sockets[i].fd);
                            number_of_clients--;
                            printusers(number_of_clients);
                            close(sockets[i].fd);
                            sockets[i].fd=INVALID_FD;
                            operation[i]=OPERATION_DEF;
                        }
                        else{
                            write(sockets[i].fd,file_exist,sizeof(file_exist));
                            fseek(fd, 0, SEEK_END);
                            long fsize = ftell(fd);
                            fseek(fd, 0, SEEK_SET);  /* same as rewind(f); */
                            char *string =(char*) malloc(fsize);
                            fsize=fread(string, sizeof(char), fsize, fd);
                            write(sockets[i].fd, string, fsize);
                            fclose(fd);
                            free(string);
                            //Сброс соединения
                            clients_statuses[i]=CLIENT_STATUS_DEFAULT;
                            first_numbers[i]=0;
                            printf("Client (socket: %d) disconnected\n", sockets[i].fd);
                            number_of_clients--;
                            printusers(number_of_clients);
                            close(sockets[i].fd);
                            sockets[i].fd=INVALID_FD;
                            operation[i]=OPERATION_DEF;
                        }
                    }
                    else{
                        write(sockets[i].fd,"Enter first arg\0",sizeof("Enter first arg\0"));
                        clients_statuses[i]=CLIENT_STATUS_SET_OPERATION;
                    }
                }
                else if(clients_statuses[i]==CLIENT_STATUS_SET_OPERATION){//Клиент отправил код операции, принимаем первый аргумент
                    read(sockets[i].fd,&first_numbers[i],sizeof(int));
                    write(sockets[i].fd,"Enter second arg\0",sizeof("Enter second arg\0"));
                    clients_statuses[i]=CLIENT_STATUS_GET_FIRST_ARG;
                }
                else if(clients_statuses[i]==CLIENT_STATUS_GET_FIRST_ARG){//Клиент отправил первый аргумент, принимаем второй аргумент
                    //получаем второй аргумент, совершаем операцию над аргументами, высылаем результат и закрываем соединение
                    int second_operand=0;
                    read(sockets[i].fd,&second_operand,sizeof(int));
                    int result=myfunc(first_numbers[i],second_operand,operation[i]);
                    write(sockets[i].fd,&result,sizeof(int));
                    //сброс настроек сокета
                    clients_statuses[i]=CLIENT_STATUS_DEFAULT;
                    first_numbers[i]=0;
                    printf("Client (socket: %d) disconnected\n", sockets[i].fd);
                    number_of_clients--;
                    printusers(number_of_clients);
                    close(sockets[i].fd);
                    sockets[i].fd=INVALID_FD;
                    operation[i]=OPERATION_DEF;
                }
            }
        }
    }
    close(sockfd);
    return 0;
}

void domath (int sock) {
    int bytes_recv; // размер принятого сообщения
    int a,b, choice; // переменные для myfunc
    char buff[20 * 1024];
    #define str0 "Enter option:\r\n1. sum\r\n2. neg\r\n3. mult\r\n4. div\r\n5. file transfer\r\n"
    #define str1 "Enter 1 parameter\r\n"
    #define str2 "Enter 2 parameter\r\n"

    // отправляем клиенту сообщение
    write(sock, str0, sizeof(str0));
    // обработка выбора опции
    bytes_recv = read(sock,&buff[0],sizeof(buff));
    if (bytes_recv < 0) error("ERROR reading from socket");
    choice = buff[0]-'0'; // преобразование выбор опции в int

    if(choice!=5){
        // отправляем клиенту сообщение
        write(sock, str1, sizeof(str1));
        // обработка первого параметра
        bytes_recv = read(sock,&buff[0],sizeof(buff));
        if (bytes_recv < 0) error("ERROR reading from socket");
        a = atoi(buff); // преобразование первого параметра в int

        // отправляем клиенту сообщение
        write(sock,str2,sizeof(str2));
        bytes_recv = read(sock,&buff[0],sizeof(buff));
        if (bytes_recv < 0) error("ERROR reading from socket");
        b = atoi(buff); // преобразование второго параметра в int
        a = myfunc(a,b,choice); // вызов пользовательской функции
        snprintf(buff, strlen(buff), "%d", a); // преобразование результата в строку
        buff[strlen(buff)] = '\n'; // добавление к сообщению символа конца строки
        // отправляем клиенту результат
        write(sock,&buff[0], sizeof(buff));
    }
    else{//Передаём файл
        //bytes_recv = read(sock,&buff[0],sizeof(buff));
        char file_name[1024];
        sscanf(buff,"%d %s",&choice,file_name);
        if (bytes_recv < 0) error("ERROR reading from socket");
        FILE* fd=fopen(file_name,"r");
        if(fd==NULL){//файл не существует
            perror("open");
            write(sock,file_not_exist,sizeof(file_not_exist));
            //exit(EXIT_FAILURE);
            close(sock);
            return;
        }
        if(write(sock,file_exist,sizeof(file_exist))==-1){
            perror("Send file exist msg");
            close(sock);
            return;
        }//Оповещаем клиента о наличии файла
        fseek(fd, 0, SEEK_END);
        long fsize = ftell(fd);
        fseek(fd, 0, SEEK_SET);  /* same as rewind(f); */
        char *string =(char*) malloc(fsize + 1);
        // do {
            fsize=fread(string, sizeof(char), fsize, fd);
            string[fsize]=0;
            if (fsize == -1) {
                perror("read");
                close(sock);
                free(string);
                return;
            }
            if (write(sock, string, fsize) == -1) {
                perror("write");
                close(sock);
                free(string);
                return;
            }
        // } while (read_return > 0);
        fclose(fd);
        free(string);
        // snprintf(buff, strlen(buff), "%d", 1); // преобразование результата в строку
        // buff[strlen(buff)] = '\n'; // добавление к сообщению символа конца строки
        // // отправляем клиенту результат
        // write(sock,&buff[0], sizeof(buff));
    }
    
    //nclients-=1; // уменьшаем счетчик активных клиентов
    //printf("-disconnect\n");
    //printusers();
    close(sock);
    return;
}