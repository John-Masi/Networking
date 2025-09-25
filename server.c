#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <netdb.h>
#include <pthread.h>

#define PORT 9000

struct Room {
	int client_1;
	int client_2;
};

void* roomHandle(void* args){
	printf("Room is up");
	struct Room* chatRoom = (struct Room*)args;

	char buffer[1024];
	ssize_t bytes;

	while(1){

		bytes = recv(chatRoom->client_1,buffer,sizeof(buffer),0);
		if (bytes < 0)	{
			perror("");
			break;
		}
		send(chatRoom->client_2,buffer,bytes,0);

		bytes = recv(chatRoom->client_2,buffer,sizeof(buffer),0);
		if (bytes < 0) {
			perror("");
			break;
		}
		send(chatRoom->client_1,buffer,bytes,0);

	}

	free(chatRoom);


	return NULL;
}

int main(){
	//setvbuf(stdout, NULL, _IONBF, 0);

	int sock_fd, sockdff;
	int opt = 1;
	struct sockaddr_in address, client_address;
	socklen_t caddress_len= sizeof(client_address);


	// Main server file directory 
	sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if (sock_fd < 0){
		perror("GAy");
		exit(1);

	}

	printf("socket is up");

	if(setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) < 0){
		perror("Error setting up sockopts");
		exit(1);
		
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	printf("checkpoint 1 \n");

	if(bind(sock_fd,(struct sockaddr *)&address,sizeof(address)) < 0){
		perror("Gay");
		exit(1);
	}

	if(listen(sock_fd,10) < 0){
		perror("GAy");
		exit(1);
	}

	while(1) {
		printf("looking for client_1\n");
		int connection_1;
		connection_1 = accept(sock_fd,(struct sockaddr *)&client_address,&caddress_len);
		printf("Client 1 has connected\n");

		int connection_2;
		connection_2 = accept(sock_fd,(struct sockaddr *)&client_address,&caddress_len);

		struct Room* chatRoom = malloc(sizeof(struct Room));
		chatRoom->client_1 = connection_1;
		chatRoom->client_2 = connection_2;
		
		pthread_t thread;
		pthread_create(&thread,NULL,roomHandle,chatRoom);
		pthread_detach(thread);

	}

	close(sock_fd);

	return 0;

}