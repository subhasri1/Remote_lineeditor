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

int main()
{
	Show_Current_Directory cur_dir;
	List_Files_And_Folders file_list;
	Change_Directory chng_dir;
	Select_File sel_file;
	Print_Line ptr_line;
	Exit qt;

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
	{
		cout<<"connection successful...!"<<endl;
	}

	while(1)
	{
		string choice;
      	cout<<"\nRemoteLineEditor~$ ";
      	cin>>choice;
      	if(choice == "pwd")
      	{
      		cur_dir.show_current_directoy_client(sock);
      	}
		else if(choice == "ls")
      	{
			file_list.list_files_and_folder_client(sock);
		}
		else if(choice == "cd")
      	{
			chng_dir.change_directory_client(sock);
		}
		else if(choice == "select")
      	{
			sel_file.select_file_client(sock);
		}
		else if(choice == "print")
      	{
			ptr_line.print_line_client(sock);
		}
		else if(choice == "bye")
      	{
			qt.good_bye_client(sock);
		}
		else
		{
			cout<<"command not found..."<<endl;
		}
	}
	close(sock);
}
