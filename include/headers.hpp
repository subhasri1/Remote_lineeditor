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

#define BUFFER_SIZE 100
#define COMMAND_SIZE 15
#define FILE_SIZE 30
#define PORT_NUMBER 8021

class Commands
{
	public:			
		struct stat sfile;					
		char buf[BUFFER_SIZE], filename[FILE_SIZE];
		int size, filehandle;
};

class Show_Current_Directory : public Commands
{
	char cwd[BUFFER_SIZE];
	char msg[25] = {"Directory not found.."};	
	public:
		void show_current_directoy_client(int sock);
		void show_current_directoy_server(int sock);
	
};

class List_Files_And_Folders : public Commands
{
    char ch[30];
	public:
		void list_files_and_folder_client(int sock);
		void list_files_and_folder_server(int sock);
	
};


class Change_Directory : public Commands
{
	char cwd[BUFFER_SIZE];
	char msg[BUFFER_SIZE] = {"Directory not found.."};
	public:
		void change_directory_server(char *command, int sock);
		void change_directory_client(int sock);
};

class Select_File : public Commands
{
	int i=0;
	public:
		void select_file_server(char *command, int sock);
		void select_file_client(int sock);
	
};

class Print_Line : public Commands
{
	public:
		char s[3], l[3];
		void print_line_server(char *filename, char *start, char *last, int sock);
		void print_line_client(int sock);
	
};


class Exit : public Commands
{
	int status;
	public:
		void good_bye_server(int sock);
		void good_bye_client(int sock);
	
};

