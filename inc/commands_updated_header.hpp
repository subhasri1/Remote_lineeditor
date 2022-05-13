#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <string>
#include <cstdio>
#include <csignal>
#include <fstream>
#include <vector>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <wait.h>
#include <dirent.h>

#define BUFFER_SIZE 10000
#define COMMAND_SIZE 15
#define FILE_SIZE 30
#define SIZECMD 50
#define LINE_CONTENT 512
#define PORT_NUMBER 8021
#define IP_ADDRESS "127.0.0.1"

class Socket_Programming
{
	public:					
		char buf[BUFFER_SIZE], filename[FILE_SIZE];
		int create_socket();
		struct sockaddr_in define_address();
		void bind_socket(int anySocket, struct sockaddr_in anyAddress);
		void listen_connections(int anySocket);
		int accept_connections(int anySocket, struct sockaddr_in anyAddress);
		void connect_to_server(int anySocket, struct sockaddr_in anyAddress);
};

class Show_Current_Directory : public Socket_Programming
{
	char cwd[SIZECMD];
	public:
		void show_current_directoy_client_side(int sock);
		void show_current_directoy_server_side(int sock);
	
};

class List_Files_And_Folders : public Socket_Programming
{
    char ch[SIZECMD];
	public:
		void list_files_and_folder_client_side(int sock);
		void list_files_and_folder_server_side(int sock);
	
};


class Change_Directory : public Socket_Programming
{
	char cwd[BUFFER_SIZE];
	char msg[SIZECMD] = {"Directory not found.."};
	public:
		void change_directory_server_side(char *command, int sock);
		void change_directory_client_side(int sock);
};

class Select_File : public Socket_Programming
{
	int i=0, x;
    string str, line;
    char msg[SIZECMD] = {"file not found..\n"};
	public:
		void select_file_server_side(char *filename, int sock);
		void select_file_client_side(int sock);
	
};

class Print_Line : public Socket_Programming
{
	int s, l, cnt;
    string line, line2;
    char path[SIZECMD] = {"Invalid line no. ..\n"};
    char msg[SIZECMD] = {"file not found..\n"};
	public:
		void print_line_server_side(char *filename,int sock);
		void print_line_client_side(int sock);
	
};

class Edit_Line : public Socket_Programming
{
	string line;
	int l, status;
	char msg[SIZECMD] = {"file not found..\n"};
	char line_msg[SIZECMD] = {"Invalid line no. ..\n"};
	char suc_msg[SIZECMD] = {"successfuly edited..\n"};

	public:
		void edit_line_server_side(char *filename,int sock);
		void edit_line_client_side(int sock);
};


class Exit : public Socket_Programming
{
	int status;
	public:
		void good_bye_server_side(int sock);
		void good_bye_client_side(int sock);
	
};

