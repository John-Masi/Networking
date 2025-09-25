#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 9000

int main(int argc, char* argv[]){
	int s, server_socket; 
	char buffer[1024];
	struct sockaddr_in serveraddress;

	s = socket(AF_INET,SOCK_STREAM,0); // Creating socket 

	if(s < 0){
		perror("Error creating Ipv4 socket");
		exit(1);
		return 1;
	}

	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(PORT);
	serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(s, (struct sockaddr *)&serveraddress,sizeof(serveraddress))< 0){	// Attemping to make connecting to our server
		perror("Failure connecting to server");
		exit(1);
		return 1;
	}
	
	printf("Ipv4 socket has connected to 127.0.0.1 on port 9000\n");
	char *m = "Gay";
	ssize_t bytes;
	memset(buffer,0,sizeof(buffer));
	while (1){
		send(s,buffer,bytes,0);
		print("Sent message\n"); // Sending a messages after we connect 

		int r = recv(s,buffer,sizeof(buffer)-1,0); // Reciving messages from the server we are connected to 
		if (r <= 0){
			perror("Could not receive message");
			exit(1);
			return 1;
		}

		send(s,buffer,r,0);
		print("Sent message\n");

		buffer[r] = '\0';
		printf("Message: %s\n",buffer);
	}

	return 0;


}