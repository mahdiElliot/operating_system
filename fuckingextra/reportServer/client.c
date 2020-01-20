#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 50 
#define PORT 40
#define SOCKETADDR struct sockaddr 

void send(int soc) 
{ 
	char buffer[MAX]; 
	bzero(buffer,MAX);
    char *b = "report cg1";
    strcpy(buffer,b);
    write(soc, buffer, sizeof(buffer));//write
		
} 

int main() 
{ 
	int soc; 
	struct sockaddr_in servaddr;
    struct sockaddr_in client; 

	// creating socket and verifying 
	soc = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM : for TCP sockets
	if (soc == -1) { 
		printf("socket creation failed!\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created \n");

	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET;  //for standard socket programming
	servaddr.sin_addr.s_addr = inet_addr("10.18.7.54"); 
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(soc, (SOCKETADDR*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed!\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server!\n"); 

	// sending data 
	send(soc); 

	// close the socket 
	close(soc); 
} 
