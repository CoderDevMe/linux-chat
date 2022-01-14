#include "UDPClient.h"
using namespace std;

#define PORT     8080
#define MAXLINE 1024 
  
// Driver code 
int main() { 
	char ip[]="127.0.0.1";
	
	UDPClient* client = new UDPClient();
	client->connect(ip,PORT);
	
	while(1){
		sleep(2);
		uint8_t buff[] = {1,2,3,4,5,6,7,8,9,10};
		client->send(buff, sizeof(buff));
	}
		
    return 0; 
}

