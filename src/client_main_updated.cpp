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

	//Creates socket for authentication
	int client_sock = cmd.create_socket(); 

	//Defines address information for authentication address 
	struct sockaddr_in Address = cmd.define_address(); 

	// Connects to Server
	cmd.connect_to_server(client_sock, Address);

	while(1)
	{
		string choice;
		cout<<"RemoteLineEditor~$ ";
      	cin>>choice;
      	if(choice == "pwd")
      		cur_dir.show_current_directoy_client_side(client_sock);
		else if(choice == "ls")
			file_list.list_files_and_folder_client_side(client_sock);
		else if(choice == "cd")
			chng_dir.change_directory_client_side(client_sock);
		else if(choice == "select")
			sel_file.select_file_client_side(client_sock);
		else if(choice == "print")
			ptr_line.print_line_client_side(client_sock);
		else if(choice == "edit")
			edt_ln.edit_line_client_side(client_sock);
		else if(choice == "bye")
			qt.good_bye_client_side(client_sock);
		else
			cout<<"command not found..."<<endl;
	}
	close(client_sock);
}
