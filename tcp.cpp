#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include <atomic>

class TCP_Server{
	int server_fd;
	int port;


public:
	TCP_Server(int p) : port(p) {
		server_fd = socket(AF_INET,SOCK_STREAM,0);

		sockaddr_in address{};

		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		// Host to network short will convert the cpus port byte number into the correct format on little endian systems
		address.sin_port = htons(port);

		if(bind(server_fd,(sockaddr*)&address,sizeof(address)) < 0){
			std::cout << "Error";
		}

		if(listen(server_fd,5) < 0) {
			std::cout << "Error";
		}
	}

	void run() {
		while(true) {
			sockaddr_in client_address{};
			socklen_t addr_len = sizeof(client_address);
			int client_fd = accept(server_fd,(sockaddr*)&client_address,&addr_len);
			if(client_fd < 0){

			}

			//count.fetch_add(1,std::memory_order_seq_cst);
			std::thread(&TCP_Server::handle_client,this,client_fd).detach();
		}
	}

	void handle_client(int client_fd) {
		char buffer[1024] = {0};
		int bytes = read(client_fd,buffer,sizeof(buffer));

		std::string ack = "Hello";
		send(client_fd,ack.c_str(),ack.size(),0);
		close(client_fd);
	}

	~TCP_Server() {
		close(server_fd);
	} 
};

int main() {
	TCP_Server s(8080);
	s.run();

}