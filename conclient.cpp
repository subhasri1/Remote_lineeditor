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



















		
	/*
	if(menuFunction(option,authSocket)==1) // If user log in successfully, close authentication socket, create new socket
	{
		genToken = recvToken(authSocket);
		int n = genToken.length(); 
    	char tokenChar[n + 1]; 
    	strcpy(tokenChar, genToken.c_str());
		string decodedToken = base64_decode(tokenChar);
		cout << "You have access to the main server until :"<< decodedToken << endl;
		close(authSocket);					// and connect to main server.
		authSocket = -1;
		clientSocket = createSocket();
		connectToMainServer(clientSocket,mainServerAddress);
		controlSendToken(clientSocket,genToken);

		pthread_t thread;
		pthread_create(&thread, NULL, receiveMessages, (void *)&clientSocket); //creating thread that receive messages for each client.

		insideMenu();
	
		while (1)
		{

			char input[BUFFERSIZE];
			cin >> input;
			if (strcmp(input, "EXIT") == 0) // Send "EXIT" command to the server.
			{
				break;
			}

			if (strcmp(input, "LIST") == 0)
			{
				
				send(clientSocket, input,BUFFERSIZE, 0); //Send LIST command to server.
			}
			if (strcmp(input, "SEND") == 0)
			{
				
				send(clientSocket, input, BUFFERSIZE, 0); //Send SEND command to server.

				cin >> input;
				send(clientSocket, input, BUFFERSIZE, 0); //Send ID of other client to server for communicating.


				cin >> input ;
				string encodee = input;
				string encodedMessage = base64_encode(reinterpret_cast<const unsigned char*>(encodee.c_str()),encodee.length());
				int n = encodedMessage.length(); 
    			char char_array[n + 1]; 
    			strcpy(char_array, encodedMessage.c_str()); // Encode the input end send encoded message to the server.
				send(clientSocket, char_array, BUFFERSIZE, 0);
				saveHistoryToDB(encodedMessage, getCurrentTimeForDB());
			}
			if (strcmp(input, "HISTORY") == 0)
			{
				//showHistory();
				send(clientSocket, input,BUFFERSIZE, 0);
			}
		}
	}	
	*/









