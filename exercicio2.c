#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define TRUE 1

int main(){

	int clientSocket, ret;
	struct sockaddr_in client_Addr;
	char buffer[1024];
	char buffer_in [1024]; 

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("error in connection....\n");
		exit(1);
	}
	printf("client Socket is created....\n");

	memset(&client_Addr, '\0', sizeof(client_Addr));
	client_Addr.sin_family = AF_INET;
	client_Addr.sin_port = htons(PORT);
	client_Addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&client_Addr, sizeof(client_Addr));
	if(ret < 0){
		printf("error in connection.\n");
		exit(1);
	}

	 
	printf("IP address is: %s\n", inet_ntoa(client_Addr.sin_addr));
	recv(clientSocket, buffer_in, 1024, 0);
	printf("message from server: %s\n ", buffer_in);


 
	while(TRUE){
		printf("Client: \t");

		scanf("%s", &buffer[0]);

		send(clientSocket, buffer, 1024,0);		
		 
		memset(buffer, 0x0, 1024);
		recv(clientSocket, buffer_in, 1024, 0);
 
		
		if(strcmp(buffer_in, "OK") != 0){
			printf(" Has a problem on connection...");
			close(clientSocket);
			exit(EXIT_FAILURE);
		}
		printf("message from server: %s \n",buffer_in );
			 
	}

	return 0;
}
