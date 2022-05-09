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

#define BUFFER_SIZE 120
#define COMMAND_SIZE 15
#define FILE_SIZE 30
#define PORT_NUMBER 8021
#define IP_ADDRESS "127.0.0.1"




//Functions

//void userRegistration();
//void login();
//void menuFunction();
//bool isBlacklist(int client_address);



/*

void menuFunction()
{
    cout << "Press : \t 1.Register\t 2.Login\t 3.Exit" << endl;
    int option;
    cin >> option;
    if (option == 1)
    {
        userRegistration();
    }
    else if (option == 2)
    {
        login();
    }
    else if (option == 3)
    {
        exit(0);
    }
    else
    {
        cout << "Wrong choice";
        exit(0);
    }
}



bool isBlacklist(string client_ip)
{
    string black_list_ip;
    ifstream file;
    file.open("etc/edit/client_blacklist.txt");
    if (file.is_open())
    {
        while(getline (file, black_list_ip))
        {
            if (black_list_ip == client_ip)
            {
                file.close();
                return true;
            }
        }
        file.close();
        return false;      
    }
}




void userRegistration()
{
	string username, password, usnm, pssd;
	cout << "Enter Username : "; cin>>username;
	cout << "Enter Password : "; cin>>password;
	
	ofstream file;
	file.open("etc/edit/userlist.txt");
    while(file.is_open())
    {
        getline (file, usnm);
        getline (file, pssd);
        if(usnm != username)
            cout<<"Invalid username\n";
        else if(pssd != password)
            cout<<"Invalid password\n";
        else if(usnm == username && pssd == password)
        {
            file.close();
            file.open("etc/edit/userlist.txt", ios::app); // Append mode    O_APPEND
            file << username << endl << password <<endl;
            cout<<"User Registration sccessful....!"<<endl;
	        file.close();
        }   
    }
}


void login()
{

    string username, password, usnm, pssd;
    int loginAttempt=0, totalAttempt=3;
    ifstream file;
    while(loginAttempt<totalAttempt)
    {
        cout << "Enter Username : "; cin>>username;
        cout << "\nEnter Password : "; cin>>password;
        file.open("etc/edit/userlist.txt");
        while(file.is_open())
        {
            getline (file, usnm);
            getline (file, pssd);
            if(usnm != username)
            {
                cout<<"Invalid username\n";
                ++loginAttempt;
                cout << "Login attempt Left : " << totalAttempt-loginAttempt << endl;
                file.close();
                break;
            }
            else if(pssd != password)
            {
                cout<<"Invalid password\n";
                ++loginAttempt;
                cout << "Attempt Left : " << totalAttempt-loginAttempt << endl;
                file.close();
                break;
            }
            else if(usnm == username && pssd == password)
            {
                cout<<"Welcome you.."<<endl;
                file.close();
                break;
            }   
        }
        if (loginAttempt == totalAttempt)
        {
            cout << "Too many login attempts.....! \nThank you for logging in.\n";
            exit(0);
        }
    }
}
*/


class Commands
{
	public:
		ofstream foutlog;			
		struct stat sfile;					
		char buf[BUFFER_SIZE], filename[FILE_SIZE];
		int size, filehandle;

		int create_socket();
		struct sockaddr_in define_address();
		void bind_socket(int anySocket, struct sockaddr_in anyAddress);
		void listen_connections(int anySocket);
		int accept_connections(int anySocket, struct sockaddr_in anyAddress);
		void connect_to_server(int anySocket, struct sockaddr_in anyAddress);
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
	int i=0;
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


