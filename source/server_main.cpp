#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <csignal>
#include <vector>
#include <fstream>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>

#include "../include/commands.hpp"

void sighandler(int signum)
{
	cout << strsignal(signum) << endl;
	wait(NULL);
}

int main()
{
	Show_Current_Directory cur_dir;
	List_Files_And_Folders file_list;
	Change_Directory chng_dir;
	Select_File sel_file;
	Print_Line ptr_line;
	Exit qt;
	Commands cmd;

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
			while(1)
			{
				int n=recv(client_sock, cmd.buf, BUFFER_SIZE, 0);
				cmd.buf[n]='\0';
		      	char *command1=strtok(cmd.buf," ");
		   		char *filename=strtok(NULL," ");
		   		char *start=strtok(NULL," ");
		   		char *last=strtok(NULL," ");
		   		if(strcmp(command1, "pwd")==0)
				{
			    	cur_dir.show_current_directoy_server(client_sock);
			    }
			    else if(strcmp(command1, "ls")==0)
				{
			    	file_list.list_files_and_folder_server(client_sock);
			    }
			    else if(strcmp(command1, "cd")==0)
				{
				    chng_dir.change_directory_server(filename, client_sock);
				}   
				else if(strcmp(command1, "select")==0)
				{
				    sel_file.select_file_server(filename, client_sock);
				}
				else if(strcmp(command1, "print")==0)
				{    
				    ptr_line.print_line_server(filename, start, last, client_sock);
				}
				if(strcmp(command1, "bye")==0)
				{    
				    qt.good_bye_server(client_sock);
				}
				bzero(cmd.buf, sizeof(cmd.buf));
			}
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
