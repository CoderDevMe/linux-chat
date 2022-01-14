#include "UDPClient.h"

UDPClient::UDPClient(){
}

UDPClient::~UDPClient(){
	this->disconnect();
}

void UDPClient::connect(char* serverIP, int port){
	// Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        cout<<("socket creation failed"); 
        return; 
    }
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    
	 // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = inet_addr(serverIP); 
    servaddr.sin_port = htons(port); 
    
    cout << "Socket creation success" << endl;
    
    // send start to server
	char startMsg[] = "start";
	if(firstSend(startMsg, 5) > 0){
		isConnected = true;
		
		receiveThread = thread(&UDPClient::receiveLoop, this);
		cout<<"Udp Connected to server success"<<endl;
	}
}


void UDPClient::receiveLoop(){
	while(isConnected){
		uint8_t _buff[512];
		int _buffLen = sizeof(_buff) + 1;
		memset(_buff, '\0', sizeof(_buff));
		
		int n = recvfrom(sockfd, (uint8_t *)_buff, _buffLen, MSG_WAITALL, 
				( struct sockaddr *) &servaddr, (socklen_t*)&serverLen);
		
		if(n <= 0){
			cout << "Udp receiving failed" << endl;
			isConnected = false;
			break;
		}
		
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


void UDPClient::disconnect(){
	isConnected = false;
	receiveThread.join();
	close(sockfd);
}


int UDPClient::send(uint8_t* buff, int count){
	if(!isConnected){
		cout<<"Udp can't send, client disconnected"<<endl;
		return 0;
	}
	
	int n = sendto(sockfd, (const uint8_t *)buff, count, MSG_CONFIRM,
			(const struct sockaddr *) &servaddr, serverLen);
	
	if(n <= 0){
		cout << "Udp send failed" << endl;
		return n;
	}
	
	memset(buff, '\0', sizeof(buff));
	cout << "send succeeded" << endl;
	return n;
}

int UDPClient::firstSend(char* buff, int count){
	int n = sendto(sockfd, (const char *)buff, count, MSG_CONFIRM,
			(const struct sockaddr *) &servaddr, serverLen);
	return n;
}
	
uint8_t UDPClient::readByte() {
	if (rcvBuff.size() <= 0)
		return '\0';
		
	uint8_t c = rcvBuff.front();
	rcvBuff.pop_front();
	
	return c;
}

void UDPClient::readAllBuffer(uint8_t* result) {
	if (rcvBuff.size() <= 0)
		return;

	memset(result, '\0', sizeof(result));
	
	int n= rcvBuff.size();
	
	for (int i = 0; i < n; i++) {
		result[i] = rcvBuff.front();
		rcvBuff.pop_front();
	}
}

int UDPClient::getBytesAvailableCount() {
	return (rcvBuff.size());
}
