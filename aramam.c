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
	char	rx_buf[258];
	char	tx_buf[258];
	int	clientfd;
	int	serverfd;
	int	listenfd;
	struct	sockaddr_in client_sock;
	struct	sockaddr_in server_sock;

	if (((*argv[1] == 'c') || (*argv[1] == 'C')) && argc == 3)
	{
		client_sock.sin_family = AF_INET;
		client_sock.sin_port = htons(4096);
		client_sock.sin_addr.s_addr = inet_addr(argv[2]);

		clientfd = socket(AF_INET, SOCK_STREAM, 0);
		if (clientfd >= 0)
		{
			printf("Client socket created successfully\n");
			printf("Trying connecting to server\n");
			if (connect(clientfd, (struct sockaddr*)&client_sock, sizeof(client_sock)) == 0)
			{
				printf("Connected to server\n");
				printf("You are client now\n");
				while (1)
				{
					fgets(tx_buf, sizeof(tx_buf), stdin);
					if (tx_buf[0] == 0x24)
					{
						write(clientfd, tx_buf, strlen(tx_buf) + 1);
						printf("Connection ended\n");
						break ;
					}
					write(clientfd, tx_buf, strlen(tx_buf) + 1);
					read(clientfd, rx_buf, sizeof(rx_buf));
					if (rx_buf[0] == 0x24)
					{
						printf("Connection ended by server\n");
						break ;
					}
					write(1, rx_buf, strlen(rx_buf));
				}
				close(clientfd);
				return (0);
			}
			else
				printf("Couldn`t connected to server\n");
			close(clientfd);
		}
		else
			printf("Unable to create client socket\n");
		printf("Exiting...\n");
	}
	else if ((*argv[1] == 's') || (*argv[1] == 'S'))
	{
		server_sock.sin_family = AF_INET;
		server_sock.sin_port = htons(4096);
		server_sock.sin_addr.s_addr = INADDR_ANY;

		listenfd = socket(AF_INET, SOCK_STREAM, 0);
		if (listenfd >= 0)
		{
			printf("Listen socket created successfully\n");
			if (bind(listenfd, (struct sockaddr*)&server_sock, sizeof(server_sock)) == 0)
			{
				printf("Listen socket successfully bind to a network interface\n");
				if (listen(listenfd, 10) == 0)
				{
					printf("Server listening...\n");
					serverfd = accept(listenfd, NULL, NULL);
					if (serverfd >= 0)
					{
						printf("A client connected\n");
						printf("You are server now\n");
						while (1)
						{
							read(serverfd, rx_buf, sizeof(rx_buf));
							if (rx_buf[0] == 0x24)
							{
								printf("Connection ended by client\n");
								break ;
							}
							write(1, rx_buf, strlen(rx_buf));
							fgets(tx_buf, sizeof(tx_buf), stdin);
							if (tx_buf[0] == 0x24)
							{
								printf("Connection ended\n");
								write(serverfd, tx_buf, strlen(tx_buf) + 1);
								break;
							}
							write(serverfd, tx_buf, strlen(tx_buf) + 1);
						}
						close(serverfd);
						close(listenfd);
						return (0);
					}
					else
						printf("Unable to accept remote connection\n");
				}
			}
			else
				printf("Unable to bind listen socket\n");
			close(listenfd);
		}
		else
			printf("Unable to create listen socket\n");
		printf("Exiting...\n");
	}
	else
		printf("Invalid option\n");
	return (0);
}
