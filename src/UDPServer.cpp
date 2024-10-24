// Server side implementation of UDP client-server model 
// https://www.geeksforgeeks.org/udp-server-client-implementation-c/
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

using std::string;

#define PORT	 8080 
#define MAXLINE 1024 

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	const char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if ( sockfd < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
    int _result = bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
	if (  _result < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	socklen_t len;
    int n; 

	len = sizeof(cliaddr); //len is value/result 

	n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
				MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
				&len);

    std::cout << "len: " << len << std::endl;
    
	buffer[n] = '\0'; 

	std::cout << "Client: " << buffer << std::endl; 
	
    sendto(sockfd, (const char *)hello, strlen(hello), 
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
			len);
	
	string datagram = std::string(buffer);
    
	std::cout<<"Hello message sent."<<std::endl; 
	return 0; 
}
