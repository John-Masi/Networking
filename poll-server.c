#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>

#define PORT 9000

int main(){
    fd_set fds;
    fd_set read_fds;

    struct sockaddr_in address;
    int server_fd;

    server_fd = socket(AF_INET,SOCK_STREAM,0)
    if(server_fd < 0) {
        perror("")
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.a_addr = inet_addr("127.0.0.1");

    if(bind(server_fd) < 0){

    }

    if(listen(server_fd,0) < 0) {

    }


}
