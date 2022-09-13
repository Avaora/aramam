#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	int	serverfd;
	int	connfd;
	char	msg[514];
	struct sockaddr_in server_sock;

	server_sock.sin_family = AF_INET;
	server_sock.sin_port = htons(4096);
	server_sock.sin_addr.s_addr = INADDR_ANY;

	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd == -1)
	{
		printf("ERROR:unable to create socket");
		return (0);
	}
	bind(serverfd, (struct sockaddr*)&server_sock, sizeof(server_sock));
	listen(serverfd, 10);
	connfd = accept(serverfd, NULL, NULL);
	if (connfd ==  -1)
	{
		printf("ERROR:connection not established");
		return (0);
	}
	while (1)
	{
		read(connfd, msg, sizeof(msg));
		write(1, msg, sizeof(msg));
	}
	return (0);
}
