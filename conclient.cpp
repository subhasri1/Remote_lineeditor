#include "functions.h"

#define SERVERGATEPORT 8021

// client sends a string to the server and it recieves the  string with greetings from the server
int main()
{
	//Creates socket for authentication
	int authSocket = createSocket(); 

	//Defines address information for authentication address
	struct sockaddr_in serverGateAddress = defineAddress(SERVERGATEPORT); 

	// Connects to Server Gate
	connectToServerGate(authSocket, serverGateAddress); 

	//cout << "clinet_port : " << serverGateAddress.sin_port << endl;
	
	//user authentication
	menuFunction();

	//conversation with server
	transact_with_server(authSocket);

}




