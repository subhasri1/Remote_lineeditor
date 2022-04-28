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
	if(connectToServerGate(authSocket, serverGateAddress)==-1)
	{
		perror("Unable to connect");
		exit(1);
	} 
	else
		cout<<"conected";
	//user authentication
	menuFunction();

	//conversation with server
	transact_with_server(authSocket);

}




