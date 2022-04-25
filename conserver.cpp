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
		cout << "server waiting for client to connect"  << endl;
	
		//accepting client request
		int client_fd = acceptConnections(serverGateSocket, serverGateAddress);

		cout << "client_ip   : " << serverGateAddress.sin_addr.s_addr << endl;
		//cout << "clinet_port : " << sock_addr_cli.sin_port << endl;
//		if(allowedIP)	
//		{
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
/*		}
		else
		{
			cout<<"user is not allowed to connect";
		}
*/			
	}	
	//close socket
	if (close(serverGateSocket)==-1)
	{
		perror("socket close error");
		exit(1);
	}
}

