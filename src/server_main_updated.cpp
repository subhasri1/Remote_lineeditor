#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <csignal>
#include <fstream>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <wait.h>
#include <dirent.h>

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
	Edit_Line edt_ln;
	Exit qt;
	Socket_Programming cmd;

	//signal disposition
	signal(SIGCHLD, sighandler);

	//Creating socket file descriptor for the server gate	
	int server_sock = cmd.create_socket();

	//Defines address information for server gate address
    struct sockaddr_in server_address = cmd.define_address(); 

    // Bind server gate socket
	cmd.bind_socket(server_sock, server_address);     

	// Listen for connections from server gate socket
	cmd.listen_connections(server_sock); 

	while (1)
	{
		//accepting client request
		int client_sock = cmd.accept_connections(server_sock, server_address);
		pid_t pid= fork();

		if (pid==0)
		{
			while(1)
			{
				int n=recv(client_sock, cmd.buf, sizeof(cmd.buf), 0);
		      	char *command1=strtok(cmd.buf," ");
		   		char *command2=strtok(NULL," ");
		   		if(strcmp(command1, "pwd")==0)
			    	cur_dir.show_current_directoy_server_side(client_sock);
				else if(strcmp(command1, "ls")==0)
					file_list.list_files_and_folder_server_side(client_sock);
				else if(strcmp(command1, "cd")==0)
				    chng_dir.change_directory_server_side(command2, client_sock);
				else if(strcmp(command1, "select")==0)
				    sel_file.select_file_server_side(command2, client_sock);
				else if(strcmp(command1, "print")==0)
				    ptr_line.print_line_server_side(command2, client_sock);
				else if(strcmp(command1, "edit")==0)
				    edt_ln.edit_line_server_side(command2, client_sock);
				if(strcmp(command1, "bye")==0)
				    qt.good_bye_server_side(client_sock);
			}
			exit(0);
		}
		else if (pid > 0)
			close(client_sock);
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
