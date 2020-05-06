#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>    
#include <arpa/inet.h>     
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h> 
#include <sys/time.h>
 
 
#define PORT 8080
#define LENGTH 1024


int main (int argc, char *argv[]){

	int server, addrlength, activity, new_socket, opt = 1;

	struct sockaddr_in address;
	
	if( (server = socket(AF_INET , SOCK_STREAM , 0)) == -1){
		printf("error in socket..");
		exit(EXIT_FAILURE);	
	}
	
	    if( (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) ==  -1 ){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(PORT);

	if(bind(server,(struct sockaddr *) &address, sizeof(address)) < 0 ){
		printf ("error in bind...");
		exit (EXIT_FAILURE);		
	}
	if (listen (server, 1024) == -1 ){
		printf ("error in listens");
		exit (EXIT_FAILURE);
	}

	addrlength = sizeof(address);
		

	fd_set  sockets;
	FD_ZERO (&sockets);
	FD_SET(server, &sockets);
 
	char buffer[LENGTH];

	while (1){
		fd_set copy = sockets;
		activity = select (FD_SETSIZE, &copy, NULL, NULL, NULL);
		for(int i = 0; i<FD_SETSIZE; i++){

			if(FD_ISSET(i, &copy)){
				
				if (server == i){
					new_socket = accept(server, (struct sockaddr *)&address, (socklen_t*)&addrlength);

					FD_SET(new_socket, &sockets);
					printf ("you have a new connection\n");

					send(new_socket, "Welcome to our Server!", LENGTH, 0);
 
				}else {
					recv(i, buffer, LENGTH,0);
					send(i, "OK", LENGTH, 0);
		        	printf ("\nclient send a message: %s\n", buffer);
				
				  }	
			}		
		}

	}

	
return 0;

}
