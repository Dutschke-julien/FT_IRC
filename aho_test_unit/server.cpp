#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

int main(){
	struct sockaddr_in sock_addr;
	std::cout << "socket\n";

	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;

	setsockopt(sock_fd, SOL_SOCKET,
			   SO_REUSEADDR | SO_REUSEPORT, &opt,
			   sizeof(opt));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(8080);
	sock_addr.sin_addr.s_addr = htons(INADDR_ANY);

	std::cout << "bind\n";
	bind(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr));

	std::cout << "listen\n";
	listen(sock_fd, 5);

	std::cout << "accept\n";
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_len);
	while (true) {
	char buf[1024];
		recv(client_fd, buf, sizeof(buf), 0);
		std::cout << buf << std::endl;
		send(client_fd, "ok", 2, 0);
	}
	close(client_fd);
	close(sock_fd);

	return 0;
}
