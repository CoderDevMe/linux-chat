
// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <iostream>
#include "UDPServer.h"


using namespace std;

#define PORT     8080
#define MAXLINE 1024 
  
// Driver code 
int main() {
	UDPServer* server = new UDPServer();
	char ip[] = "127.0.0.1";
	server->connect(ip, PORT);
	while(1){
		sleep(0.5);
		int n = server->getBytesAvailableCount();
		
		if( n > 0){
			cout<<"received "<<n<<" bytes"<<endl;
			uint8_t result[50];
			server->readAllBuffer(result);
			for(int i=0; i<10; i++){
				
			printf("%d,\n",result[i]);
			}
		}
	}
	
    return 0; 
}
