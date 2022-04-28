#include "functions.h"

#define SERVERGATEPORT 8021



int main(int argc, char const* argv[])
{
	//struct sockaddr_in sock_addr_cli;
	//signal disposition
	//signal(SIGCHLD, sighandler);

	//socket creation
	int serverGateSocket = createSocket();	//Creating socket file descriptor for the server gate
	
	//Defines address information for server gate address
    struct sockaddr_in serverGateAddress = defineAddress(SERVERGATEPORT);

    // Bind server gate socket
	bindSocket(serverGateSocket, serverGateAddress); 

	// Listen for connections from server gate socket
	listenForConnections(serverGateSocket); 

	while (1)
	{	
		//accepting client request
		int client_fd = acceptConnections(serverGateSocket, serverGateAddress);
		string ip = inet_ntoa((in_addr)serverGateAddress.sin_addr);
		bool isBlacklist(ip);
		if(isBlacklist == false)	
		{
			pid_t pid= fork();
			if (pid==0)
			{
				transact_with_client(client_fd);
				exit(0);
			}
			else if (pid > 0)
				close(serverGateSocket);
			else
			{
				perror("error in fork");
				exit(1);
			}
		}	
	}	
	//close socket
	if (close(serverGateSocket)==-1)
	{
		perror("socket close error");
		exit(1);
	}
}

