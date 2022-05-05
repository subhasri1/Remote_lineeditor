#include "../include/functions.hpp"

void sighandler(int signum)
{
	cout << strsignal(signum) << endl;
	wait(NULL);
}


int main()
{
	SERVER serv_obj;

	//signal disposition
	signal(SIGCHLD, sighandler);

	//socket creation
	int server_sock = socket(AF_INET,SOCK_STREAM,0);
	if (server_sock == -1)
	{
		perror("socket creation error");
		exit(1);
	}

	//initialize the sockaddr_in structure
	struct sockaddr_in server_address;			// sockaddr_in for IPV4
	server_address.sin_family = AF_INET;
	server_address.sin_port = PORT_NUMBER;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	//bind the socket with ip address and port
	if (bind(server_sock,(struct sockaddr *)&server_address,sizeof(server_address)) == -1)
	{
		perror("bind error");
		exit(1);
	}

	// listening (waiting) for client request
	if (listen(server_sock, 5) == -1)
	{
		perror("listen error");
		exit(1);
	}

	while (1)
	{
		cout << "server waiting for client to connect"  << endl;
		
		//accepting client request
		struct sockaddr_in client_address;
		socklen_t address_len = sizeof(client_address);
		int client_sock = accept(server_sock, (struct sockaddr *)&client_address, &address_len);
		
		if (client_sock == -1)
		{
			perror(" accept error");
			exit(1);
		}
		
		pid_t pid= fork();
		if (pid==0)
		{
			serv_obj.transact_with_client(client_sock);
			exit(0);
		}
		else if (pid > 0)
		{
			close(client_sock);
		}
		else
		{
			perror("error in fork");
			exit(1);
		}	
	}	

	//close socket
	if (close(server_sock)==-1)
	{
		perror("socket close error");
		exit(1);
	}
}

