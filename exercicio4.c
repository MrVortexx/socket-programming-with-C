#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>    
#include <arpa/inet.h>     
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h> 
#include <sys/time.h>
#include <time.h> 
#include <sys/select.h>


 
 
#define PORT 8080
#define LENGTH 1024
#define MAX 10
typedef struct{
	char ip [20];
	//the value is just to leave the list in an orderly manner
	int value;
		
}connected_ip;
typedef struct {

	connected_ip ip_in_list[MAX];
	int total_ips;

}ip_list;

void start_list (ip_list* list){

	list->total_ips = 0;
	
}

 

int num_ips (ip_list* list){

	return list->total_ips;
}

int search_ip( ip_list* list, int value ){
	int left, right, half;
	left = 0;
 
	right =(list->total_ips) -1;
 
 	while(left <= right){
		half =(left+right)/2;
			 

		if(list->ip_in_list[half].value == value ){

		return half;

		}else {
			if(list->ip_in_list[half].value<value){
			left = half + 1;
			}else{ 
				right = half -1;
			 }		
			
		}
 
	}
	 
	return -1;
}

int insert_elements(ip_list* list, char ip[], int value){
	if (list->total_ips == MAX ){
		return -1;
	}
	int lastElement = list->total_ips;
	int i;
	for(i = lastElement ; i>0 && list->ip_in_list[i -1].value > value; i--){
		list->ip_in_list[i].value = list->ip_in_list[i - 1].value;
		strcpy (list->ip_in_list[i].ip, list->ip_in_list[i -1].ip);
	}

 
	strcpy (list->ip_in_list[i].ip, ip);
	list->ip_in_list[i].value = value;
	list->total_ips++;
 
}


void show_ips(ip_list* list){
	int i;
	for(i = 0; i < list->total_ips; i++){
		printf("%s \n", list->ip_in_list[i].ip);
	}
 }

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
 	//start data structure
	ip_list ip_list;
	start_list(&ip_list);


	while (1){
		fd_set copy = sockets;
		activity = select (FD_SETSIZE, &copy, NULL, NULL, NULL);
		for(int i = 0; i<FD_SETSIZE; i++){

			if(FD_ISSET(i, &copy)){
				//new socket connections
				if (server == i){
					new_socket = accept(server, (struct sockaddr *)&address, (socklen_t*)&addrlength);
 
 

						if(num_ips(&ip_list)){
							send(new_socket, "You are trying to connect in a full server...", LENGTH, 0);
							printf("someone try to connect, but you server are full..");	
						}else{
 				
							char copy[20];
							int search ;
		
							// taking the ip of the client							
							strcpy(copy, inet_ntoa(address.sin_addr));
		
 							//verify if ip is on the list or not
							search = search_ip ( &ip_list, num_ips(&ip_list) );
							if(search == -1){
								// the insert will be at last free position == num_ips()
								insert_elements(&ip_list, copy, num_ips(&ip_list));	
							}

 							
 
							FD_SET(new_socket, &sockets);
							printf ("you have a new connection.\n Now you have %i connections.\n If the ip does not exist in the list, it has been added.\n Actual ips: ", num_ips(&ip_list));
							show_ips(&ip_list);
							send(new_socket, "Connected", LENGTH, 0);
							printf("%s", copy);
						 }

		 			//existing socket sending informations
				}else {	
						
					recv(i, buffer, LENGTH,0);
						
			 			if(strcmp(buffer, "HELLO") !=0){

							send(i, " You only can send Hello, desconecting...",LENGTH,0);	
							printf("A socket has ben sended a w" );
							FD_CLR(i, &sockets);
							printf("A socket has ben sended a w" );
							close(i);

						}else{
						send(i, "OK", LENGTH, 0);
		        		printf ("Message acepted! client send a message: %s\n", buffer);
					}
					
				  }	
			}		
		}

	}

	
return 0;

}
