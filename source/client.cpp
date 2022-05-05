#include "../include/functions.hpp"

int main()
{
	CLIENT cli_obj;


	//socket creation
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if (sock == -1)
	{
		perror("socket creation error");
		exit(1);
	}

	//initialize the socaddr_in structure
	struct sockaddr_in address;// sockaddr_in for IPV4
	address.sin_family = AF_INET;
	address.sin_port = PORT_NUMBER;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");

	// connecting to server
	socklen_t address_len = sizeof(address);
	if (connect(sock, (struct sockaddr *)&address, address_len)==-1)
	{
		perror("connect error");
		exit(1);
	}
	else
		cout<<"connection successful...!"<<endl;

	cli_obj.transact_with_server(sock);

	close(sock);
	
}
