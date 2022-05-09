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
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include "headers.hpp"

int main()
{
	ofstream foutLog;
	foutLog.open("client.log",ios::app);
	Show_Current_Directory cur_dir;
	List_Files_And_Folders file_list;
	Change_Directory chng_dir;
	Select_File sel_file;
	Print_Line ptr_line;
	Exit qt;
	Commands cmd;
	foutLog<<"[Info] : Creatinng a client Socket"<<endl;
	//Creates socket for authentication
	int client_sock = cmd.create_socket(); 

	//Defines address information for authentication address 
	struct sockaddr_in Address = cmd.define_address(); 
	foutLog<<"[Info] : Connecting to server"<<endl;
	// Connects to Server
	cmd.connect_to_server(client_sock, Address);

	while(1)
	{	
		foutLog<<"[Info] : Handshaking to server"<<endl;
		string choice;
		cout<<"\nRemoteLineEditor~$ ";
      	cin>>choice;
      	if(choice == "pwd")
      		cur_dir.show_current_directoy_client(client_sock);
		else if(choice == "ls")
			file_list.list_files_and_folder_client(client_sock);
		else if(choice == "cd")
			chng_dir.change_directory_client(client_sock);
		else if(choice == "select")
			sel_file.select_file_client(client_sock);
		else if(choice == "print")
			ptr_line.print_line_client(client_sock);
		else if(choice == "bye")
			qt.good_bye_client(client_sock);
		else
			cout<<"command not found..."<<endl;
	}
	foutLog.close();
	close(client_sock);
}

