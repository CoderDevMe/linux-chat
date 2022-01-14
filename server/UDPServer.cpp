#include "UDPServer.h"

UDPServer::UDPServer(){
}

UDPServer::~UDPServer(){
	this->disconnect();
}

void UDPServer::connect(char* serverIP, int port){
	// Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        cout<<("socket creation failed"); 
        return; 
    }
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr));
	 // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = inet_addr(serverIP); 
    servaddr.sin_port = htons(port); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 )
    { 
        cout<<("bind failed");
        return;
    }
    isListning = true;
    
    receiveThread = thread(&UDPServer::receiveLoop, this);
}


void UDPServer::receiveLoop(){
	while(isListning){
		uint8_t _buff[512];
		int _buffLen = sizeof(_buff) + 1;
		memset(_buff, '\0', sizeof(_buff));
		
		int n = recvfrom(sockfd, (uint8_t *)_buff, _buffLen, MSG_WAITALL, 
				( struct sockaddr *) &cliaddr, (socklen_t*)&clientLen);
			
		if(n <= 0){
			cout << "Udp receiving failed" << endl;
			isConnected = false;
			continue;
		}
		
		isConnected = true;
		
		//copy incoming data to my queue
		for(int i=0; i< _buffLen ;i++){
			if(_buff[i] =='\0')
				break;
			rcvBuff.push_back(_buff[i]);
		}
		
		cout << "receiving succeeded" << endl;
		cout << "rcv Data count >> " << n << endl;
	}
}


void UDPServer::disconnect(){
	isConnected = false;
	isListning = false;
	receiveThread.join();
	close(sockfd);
}


int UDPServer::send(uint8_t* buff, int count){
	if(!isConnected){
		cout<<"Udp can't send, client disconnected"<<endl;
		return 0;
	}
	
	int n = sendto(sockfd, (const uint8_t *)buff, count, MSG_CONFIRM,
			(const struct sockaddr *) &cliaddr, clientLen);
	
	if(n <= 0){
		cout << "Udp send failed" << endl;
		return n;
	}
	
	memset(buff, '\0', sizeof(buff));
	cout << "send succeeded" << endl;
	return n;
}
	
uint8_t UDPServer::readByte() {
	if (rcvBuff.size() <= 0)
		return '\0';
		
	uint8_t c = rcvBuff.front();
	rcvBuff.pop_front();
	
	return c;
}

void UDPServer::readAllBuffer(uint8_t* result) {
	if (rcvBuff.size() <= 0)
		return;

	memset(result, '\0', sizeof(result));
	
	int n= rcvBuff.size();
	
	for (int i = 0; i < n; i++) {
		result[i] = rcvBuff.front();
		rcvBuff.pop_front();
	}
}

int UDPServer::getBytesAvailableCount() {
	return (rcvBuff.size());
}
