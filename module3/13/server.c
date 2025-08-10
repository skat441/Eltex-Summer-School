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
void printusers() {
    if(nclients) {
    printf("%d user on-line\n",
    nclients);
    }
    else {
    printf("No User on line\n");
    }
}
// функция обработки данных
int myfunc(int a, int b, int operation) {
         if(operation==1)return a + b;
    else if(operation==2)return a - b;
    else if(operation==3)return a * b;
    else if(operation==4)return a / b;
    else return a+b;
}

static int shutdown_server=0;

void listener(int sig) {
    if(sig==SIGINT)shutdown_server=1;
}

void sigchld_handler(int sig) {
    int status;
    pid_t pid;
    
    // Ожидаем завершения дочернего процесса
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        nclients--; // Уменьшаем счетчик при завершении дочернего процесса
        printf("-disconnect (PID: %d)\n", pid);
        printusers();
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, listener);
    signal(SIGCHLD, sigchld_handler);
    char buff[1024]; // Буфер для различных нужд
    int sockfd, newsockfd; // дескрипторы сокетов
    int portno; // номер порта
    int pid; // id номер потока
    socklen_t clilen; // размер адреса клиента типа socklen_t
    struct sockaddr_in serv_addr, cli_addr; // структура сокета сервера и клиента
    printf("TCP SERVER DEMO\n");
    // ошибка в случае если мы не указали порт
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    // Шаг 1 - создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    // Шаг 2 - связывание сокета с локальным адресом
    bzero((char*) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // сервер принимает подключения на все IP-адреса
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    // Шаг 3 - ожидание подключений, размер очереди - 5
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    // Шаг 4 - извлекаем сообщение из очереди (цикл извлечения запросов на подключение)
    while (!shutdown_server)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            if (shutdown_server) break;
            error("ERROR on accept");
        }
        // вывод сведений о клиенте
        struct hostent *hst;
        hst = gethostbyaddr((char *)&cli_addr.sin_addr, 4, AF_INET);
        printf("+%s [%s] new connect!\n",
        (hst) ? hst->h_name : "Unknown host",
        (char*)inet_ntoa(cli_addr.sin_addr));
        nclients++;
        printusers();
        pid = fork();
        if (pid < 0) error("ERROR on fork");
        if (pid == 0) {
            close(sockfd);
            domath(newsockfd);
            exit(0);
        }
        else close(newsockfd);
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
    #define file_not_exist "NOTEXIST\0"
    #define file_exist "EXIST\0"

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