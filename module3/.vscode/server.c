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
#include <sys/wait.h>

// функция обслуживания подключившихся пользователей
void domath(int);

// функция обработки ошибок
void error(const char *msg) {
    perror(msg);
    exit(1);
}

// количество активных пользователей
static int nclients = 0;

// печать количества активных пользователей
void printusers() {
    if(nclients) {
        printf("%d user on-line\n", nclients);
    }
    else {
        printf("No User on line\n");
    }
}

// функция обработки данных
int myfunc(int a, int b, int operation) {
    if(operation==1) return a + b;
    else if(operation==2) return a - b;
    else if(operation==3) return a * b;
    else if(operation==4) return a / b;
    else return a+b;
}

static int shutdown_server=0;

void listener(int sig) {
    if(sig==SIGINT) shutdown_server=1;
}

// Обработчик сигнала SIGCHLD для очистки завершенных дочерних процессов
void sigchld_handler(int sig) {
    int status;
    pid_t pid;
    
    // Ожидаем завершения дочернего процесса
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("SIGCHLD handler: before decrement, nclients = %d\n", nclients);
        if (nclients > 0) { // Защита от отрицательных значений
            nclients--; // Уменьшаем счетчик при завершении дочернего процесса
        }
        printf("-disconnect (PID: %d)\n", pid);
        printusers();
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, listener);
    signal(SIGCHLD, sigchld_handler); // Устанавливаем обработчик SIGCHLD
    
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
        
        nclients++; // Увеличиваем счетчик в родительском процессе
        printf("Parent process: nclients = %d\n", nclients);
        printusers();
        
        pid = fork();
        if (pid < 0) error("ERROR on fork");
        if (pid == 0) {
            // Дочерний процесс
            close(sockfd);
            domath(newsockfd);
            exit(0);
        }
        else {
            // Родительский процесс
            close(newsockfd);
        }
    }
    
    close(sockfd);
    return 0;
}

void domath(int sock) {
    int bytes_recv; // размер принятого сообщения
    int a, b, choice; // переменные для myfunc
    char buff[20 * 1024];
    
    #define str0 "Enter option:\r\n1. sum\r\n2. neg\r\n3. mult\r\n4. div\r\n5. file transfer\r\n"
    #define str1 "Enter 1 parameter\r\n"
    #define str2 "Enter 2 parameter\r\n"

    // отправляем клиенту сообщение
    write(sock, str0, sizeof(str0));
    
    // обработка выбора опции
    bytes_recv = read(sock, &buff[0], sizeof(buff));
    if (bytes_recv < 0) error("ERROR reading from socket");
    buff[bytes_recv] = '\0'; // Добавляем завершающий ноль
    choice = buff[0] - '0'; // преобразование выбор опции в int

    if(choice != 5) {
        // отправляем клиенту сообщение
        write(sock, str1, sizeof(str1));
        
        // обработка первого параметра
        bytes_recv = read(sock, &buff[0], sizeof(buff));
        if (bytes_recv < 0) error("ERROR reading from socket");
        buff[bytes_recv] = '\0'; // Добавляем завершающий ноль
        a = atoi(buff); // преобразование первого параметра в int

        // отправляем клиенту сообщение
        write(sock, str2, sizeof(str2));
        
        bytes_recv = read(sock, &buff[0], sizeof(buff));
        if (bytes_recv < 0) error("ERROR reading from socket");
        buff[bytes_recv] = '\0'; // Добавляем завершающий ноль
        b = atoi(buff); // преобразование второго параметра в int
        
        a = myfunc(a, b, choice); // вызов пользовательской функции
        snprintf(buff, sizeof(buff), "%d\n", a); // преобразование результата в строку
        
        // отправляем клиенту результат
        write(sock, &buff[0], strlen(buff));
    }
    else {
        // Обработка передачи файла
        char file_name[1024];
        sscanf(buff, "%d %s", &choice, file_name);
        
        int fd = open(file_name, O_RDONLY);
        if(fd == -1) {
            perror("open");
            snprintf(buff, sizeof(buff), "Error: Cannot open file %s\n", file_name);
            write(sock, buff, strlen(buff));
        } else {
            int read_return;
            do {
                read_return = read(fd, buff, sizeof(buff));
                if (read_return == -1) {
                    perror("read");
                    break;
                }
                if (read_return > 0) {
                    if (write(sock, buff, read_return) == -1) {
                        perror("write");
                        break;
                    }
                }
            } while (read_return > 0);
            close(fd);
        }
        
        snprintf(buff, sizeof(buff), "1\n"); // преобразование результата в строку
        // отправляем клиенту результат
        write(sock, &buff[0], strlen(buff));
    }
    
    close(sock);
    return;
}
