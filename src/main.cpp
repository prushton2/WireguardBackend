// Server side implementation of UDP client-server model
// https://www.geeksforgeeks.org/udp-server-client-implementation-c/
#include <bits/stdc++.h>
#include <cstdint>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "messages.h"
#include "UDPClient.h"

using std::string;

#define PORT	51820
#define MAXLINE 1024

void print_field(uint8_t* field_addr, string label, int size) {
	std::cout << label << ": ";
	for(int i = 0; i<size; i++) {
		printf("%02X", field_addr[i]);
	}
	printf("\n");
}

// Driver code
int main() {
	int sockfd;
	uint8_t buffer[MAXLINE];
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

	while(1) {
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr,
				&len);
		
		std::cout << "------------------------------" << std::endl << "Sender: ";
		printf("%d.", (cliaddr.sin_addr.s_addr >> 0)  & 0b11111111);
		printf("%d.", (cliaddr.sin_addr.s_addr >> 8)  & 0b11111111);
		printf("%d.", (cliaddr.sin_addr.s_addr >> 16) & 0b11111111);
		printf("%d\n", (cliaddr.sin_addr.s_addr >>24) & 0b11111111);
		std::cout << "Length: " << n << std::endl;

		buffer[n+1] = '\0';

		// std::cout << "Datagram: "<< std::endl;


		// printf("Hexcode:");
		// for (int i = 0; i < n; ++i) {
		// 	if(i%16 == 0)
		// 		printf("\n");
		// 	printf("%02x ", (int)(*(unsigned char*)(&buffer[i])));
		// }
		
		std::cout << std::endl;

		printf("Type: %d : ", buffer[0]);

		unsigned int start = 4;
		switch(buffer[0]) {
			case (Type::InitiatorToResponder): {
				struct InitiatorToResponder packet;
				memcpy(&packet.sender, &buffer[start], 4); start += 4;
				memcpy(&packet.ephemeral, &buffer[start], 32); start += 32;
				memcpy(&packet._static, &buffer[start], 32); start += 32;
				memcpy(&packet.timestamp, &buffer[start], 12); start += 12;
				memcpy(&packet.mac1, &buffer[start], 16); start += 16;
				memcpy(&packet.mac2, &buffer[start], 16); start += 16;
				
				std::cout << "InitiatorToResponder" << std::endl;

				print_field(packet.sender, "Sender", 4);
				print_field(packet.ephemeral, "Ephemeral", 32);
				print_field(packet._static, "Static", 32);
				print_field(packet.timestamp, "Timestamp", 32);
				print_field(packet.mac1, "Mac1", 32);
				print_field(packet.mac2, "Mac2", 32);

				sendPacket(cliaddr.sin_addr.s_addr);

				}

				break;
			case (Type::ResponderToInitiator): {
				struct ResponderToInitiator packet;
				memcpy(&packet.sender, &buffer[start], 4); start += 4;
				memcpy(&packet.receiver, &buffer[start], 4); start += 4;
				memcpy(&packet.ephemeral, &buffer[start], 32); start += 32;
				memcpy(&packet.mac1, &buffer[start], 16); start += 16;
				memcpy(&packet.mac2, &buffer[start], 16); start += 16;
				
				std::cout << "ResponderToInitiator" << std::endl;

				print_field(packet.sender, "Sender", 4);
				print_field(packet.receiver, "Receiver", 4);
				print_field(packet.ephemeral, "Ephemeral", 32);
				print_field(packet.mac1, "Mac1", 32);
				print_field(packet.mac2, "Mac2", 32);
				}
				break;
			case (Type::TransportDataMessage): {
				struct TransportDataMessage packet;
				memcpy(&packet.receiver, &buffer[start], 4); start += 4;
				memcpy(&packet.counter, &buffer[start], 8); start += 8;
				memcpy(&packet.packet, &buffer[start], 64); start += 64;
				
				std::cout << "TransportDataMessage" << std::endl;

				print_field(packet.receiver, "Receiver", 4);
				print_field(packet.counter, "Counter", 8);
				print_field(packet.packet, "Data", 64);
				}
				break;
			case (Type::CookieReplyMessage):{
				struct CookieReplyMessage packet;
				memcpy(&packet.receiver, &buffer[start], 4); start += 4;
				memcpy(&packet.nonce, &buffer[start], 24); start += 24;
				memcpy(&packet.cookie, &buffer[start], 16); start += 16;
				
				std::cout << "CookieReplyMessage" << std::endl;

				print_field(packet.receiver, "Sender", 4);
				print_field(packet.nonce, "Nonce", 8);
				print_field(packet.cookie, "Cookie", 64);
				}
				break;
			default:
				std::cout << "No recognized type" << std::endl;
				break;
		}

		sendto(sockfd, (const char *)hello, strlen(hello),
			MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
				len);
		
	}
	return 0;
}