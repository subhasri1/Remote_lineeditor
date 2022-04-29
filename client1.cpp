#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>
using namespace std;
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fstream>


int main(int argc,char *argv[])
{
  	int sock;
  	char buf[256], command[15], filename[20], file_contents[1080];
  	int status, k;
  	sock = socket(AF_INET, SOCK_STREAM, 0); //created socket
  	if(sock == -1)
    	{
      		perror("socket creation failed");
      		exit(1);
    	}
  	struct sockaddr_in server;// creating socket address
  	server.sin_family = AF_INET;
  	server.sin_port = 8021;
  	server.sin_addr.s_addr = inet_addr("127.0.0.1");
  	k = connect(sock,(struct sockaddr*)&server, sizeof(server));
  	if(k == -1)
    	{
      		perror("Connect Error");
      		exit(1);
    	}
  	int i = 1;
  	while(1)
    {	string choice;
      	cout<<"\nEditor~$ ";
      	cin>>choice;

      		
			if(choice == "pwd")
		  	{
	  			strcpy(command, "pwd");
	  			send(sock, command, 15, 0);
	  			recv(sock, buf, sizeof(buf), 0);
	 			cout<<buf;
	  		}
	  
			else if(choice == "ls")
	  		{
	  			strcpy(command, "ls"); 
          		send(sock, command, 100, 0);
          		recv(sock, buf, sizeof(buf), 0);
	 			cout<<buf;
  			}
/*	  
			else if(choice == "select")
	  		{
	            char filename[100];
	  			strcpy(buf, "select "); 
	  			cout<<"Enter filename to select within \" \": ";
	  			cin>>filename;
	  			strcat(buf, filename); 
			  	send(sock, buf, 100, 0);
			  	recv(sock, file_contents, sizeof(file_contents), 0); //receiving the path from server
	 			cout<<file_contents;		  	
	  		}
*/
			else if(choice == "cd")
	  		{
	  			strcpy(buf, "cd ");
	  			cin>>buf+3;
	  			send(sock, buf, 100, 0);
	  			recv(sock, buf, sizeof(buf), 0);
	 			cout<<buf;
          	}

			else if(choice == "bye")
			{
			  	strcpy(buf, "bye");
			  	send(sock, buf, 100, 0);
			  	recv(sock, &status, 100, 0);
			  	if(status)
			    {
			      		cout<<"Terminated\n";
			      		close(sock);
			      		exit(0);
			   	}
			}
			else
				cout<<"command not found...!";			
    }
}





