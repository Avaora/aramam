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
	int	clientfd;
	char	msg[514];
	struct sockaddr_in client_sock;

	client_sock.sin_family = AF_INET;
	client_sock.sin_port = htons(4096);
	client_sock.sin_addr.s_addr = inet_addr("127.0.0.1");

	clientfd = socket(AF_INET, SOCK_STREAM, 0);

	if (clientfd == -1)
	{
		printf("ERROR:unable to create socket");
		return (0);
	}
	if (connect(clientfd, (struct sockaddr *)&client_sock, sizeof(client_sock)) == 0)
	{
		while (1)
		{
			fgets(msg, sizeof(msg), stdin);
			if (msg[0] == 0x24)
			{
				close(clientfd);
				return (0);
			}
			write(clientfd, msg, sizeof(msg));
		}
	}
	return (0);
}
