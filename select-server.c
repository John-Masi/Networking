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

// NOTES(Two way chatroom using multiplexing select):

// Select in multiplexing is a way to let us 

// select() - lets us iterate over more rooms
// --- FILE DESCRIPTOR funcs() --- 
// FD_SETSIZE -  is max amount of sockets we can have 
// FD_SET - adds File descriptors to the FD set
// FD_ZERO - Zeroes out fd set before initalzation 
// FD_CLR - Clear out a specific fd in the FD Set 
struct User {
	int connection;
	char name[20];
}

struct User users[1024/2];

int main(){x
	fd_set master; // Stores all fd's 
	fd_set read_fds;
	int fdmax; // Top file descriptor 
	int user_count = 0;
	int waiting_client = -1;
	int server_fd;

	struct sockaddr_in address, client_address;
	socklen_t client_addrlen = sizeof(client_address);
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	char buffer[256];

	int opt = 1;
	server_fd = socket(AF_INET,SOCK_STREAM,0);

	if(server_fd < 0){
		perror("Error creating socket");
		exit(1);
		return  1;
	}
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	address.sin_family = AF_INET;
	address.sin_port = htons(9000);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(bind(server_fd,(struct sockaddr *)&address,sizeof(address)) < 0){
		perror("");
		exit(1);
		return 1;
	}

	FD_ZERO(&master);
	FD_SET(server_fd,&master);
	fdmax = server_fd;

	if(listen(server_fd,10) < 0){
		perror("");
		exit(1);
		return 1;
	}


	while(1){
		printf("Looking for clients\n");
		read_fds = master;

		if(select(fdmax + 1,&read_fds,NULL,NULL,NULL) < 0){
			perror("select has failed");
			exit(1);
			return 1;
		}

		for(int i = 0; i <= fdmax; i++){
			if(FD_ISSET(i, &read_fds)) {
				if (i == server_fd){
					// taking in new connectins 
					int new_conn = accept(server_fd,(struct sockaddr *)&client_address,&client_addrlen);
					printf("New connection coming in \n");

					if (new_conn < 0){
						continue;
					}

					FD_SET(new_conn,&master); // Setting new fd in FD List

					if (new_conn > fdmax ) fdmax = new_conn;
					printf("New connection accepted");

					users[user_count].connection = new_conn;
					users[user_count].username = "USER1";
					user_count++;
			}
			 	else{


					int bytes = recv(i,buffer,sizeof(buffer),0);
					if(bytes < 0){
						close(i);
						FD_CLR(i,&master);
					}
					else{
						buffer[bytes] = '\0';
						printf("Message from %d",users[i].username);
						printf("%s\n",buffer);

						send(i,buffer,bytes,0);
							
					} 
				}
		}
	}
  }
}	