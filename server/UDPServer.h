#include <iostream>
#include <thread>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <deque>

using namespace std;

class UDPServer
{
public:
	UDPServer();
	~UDPServer();
	void connect(char* serverIP, int port);
	void disconnect();
	int send(uint8_t* buff, int count);
	uint8_t readByte();
	void readAllBuffer(uint8_t* result);
	int getBytesAvailableCount();

private:
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;

	//receive parameters
	deque<uint8_t> rcvBuff;
	int clientLen = sizeof(cliaddr);

	bool isConnected = false;
	bool isListning = false;
	void receiveLoop();
	thread receiveThread;
};
