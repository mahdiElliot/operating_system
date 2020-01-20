#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <string.h>

#define MAX 50 
#define PORT 40 
#define SOCKETADDR struct sockaddr 

void recieve(int soc)
{
    char buffer[MAX];
    
    bzero(buffer, sizeof(buffer)); 
	read(soc, buffer, sizeof(buffer));//read 
	printf("From client : %s\n", buffer); 
}

int main()
{
    int soc,accpt,size;
    struct sockaddr_in servaddr;
    struct sockaddr_in client;
    
    //create socket and also verify it
    soc = socket(AF_INET,SOCK_STREAM,0);
    
    if(soc == -1)
    {
        printf("socket creation failed\n");
        exit(0);
    }
    else
        printf("socket successfully created\n");

    bzero(&servaddr, sizeof(servaddr));

    //assing IP,PORT
    servaddr.sin_family =  AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("10.18.7.54");  
    servaddr.sin_port = htons(PORT); 

    // Binding newly created socket to given IP and verification 
    if ((bind(soc, (SOCKETADDR*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded\n"); 

    //server is ready to listen and verification 
    if ((listen(soc, 5)) != 0) { 
        printf("Listen failed\n"); 
        exit(0); 
    } 
    else
        printf("Server listening\n"); 

    size = sizeof(client);

    // Accept the data packet from client and verification 
    accpt = accept(soc, (SOCKETADDR*)&client, &size); 
    if (accpt < 0) { 
        printf("server acccept failed\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client\n"); 

    // recieving data
    recieve(accpt); 
  
    //close the socket
    close(soc); 
}
