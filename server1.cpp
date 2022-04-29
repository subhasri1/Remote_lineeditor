#include<iostream>
using namespace std;
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	chdir
#include <dirent.h> //for get directory


int main(int argc,char *argv[])
{
	struct sockaddr_in server, client;
    struct stat sfile;
	int sock1, sock2;
	char buf[256], file_contents[1080];
	int k, i, size, c;
	int filehandle;
	sock1 = socket(AF_INET, SOCK_STREAM, 0);
	if(sock1 == -1)
    {
      	printf("Socket creation failed");
   		exit(1);
   	}
  	server.sin_family=AF_INET;
  	server.sin_port = 8021;
  	server.sin_addr.s_addr = inet_addr("127.0.0.1");
  	k = bind(sock1,(struct sockaddr*)&server,sizeof(server));
  	if(k == -1)
    {
      	printf("Binding error");
      	exit(1);
   	}
  	k = listen(sock1,1);
  	if(k == -1)
    {
      	printf("Listen failed");
      	exit(1);
   	}
  	socklen_t len = sizeof(client);
  	sock2 = accept(sock1,(struct sockaddr*)&client, &len);
  	i = 1;
  	while(1)
    {
      	int n=recv(sock2, buf, 100, 0);
   		buf[n]='\0';
      	char *command=strtok(buf," ");
   		char *command2=strtok(NULL," ");

   		if(strcmp(command, "pwd")==0)
		{
			char cwd[256];
	  		if (getcwd(cwd, sizeof(cwd)) == NULL)
	  		{
				perror("getcwd() error");
   				char msg[100] = {"Directory not found.."};
      			send(sock2, msg, 100, 0);
      		}
    		else
			{
				send(sock2, cwd, sizeof(cwd), 0);
    		}
          			
		}
      	else if(strcmp(command, "ls")==0)
		{
			DIR *d;
			char ch[100];
    		struct dirent *dir;
    		d = opendir(".");
    		if (d)
    		{
        		while ((dir = readdir(d)) != NULL)
        		{
            		strncpy(ch, dir->d_name, 99);
            		ch[99] = '\0';
            		strcat(buf, ch);
        		}
        		closedir(d);
        		send(sock2, buf, sizeof(buf), 0);
			}
		}

      	else if(strcmp(command, "cd") == 0)
        {
        	char cwd[256];
	  		if (getcwd(cwd, sizeof(cwd)) == NULL)
			{
      			perror("getcwd() error");
      			char msg[100] = {"Directory not found.."};
				send(sock2, msg, 100, 0);
      		}

    		chdir(command2);
    		getcwd(cwd, sizeof(cwd));
			send(sock2, cwd, sizeof(cwd), 0);
    	}

        else if(!strcmp(command, "bye"))
		{
	  		
	  		i = 1;
	  		send(sock2, &i, sizeof(int), 0);
	  		exit(0);
		}
    }
}