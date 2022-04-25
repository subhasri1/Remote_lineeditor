#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wait.h>
#include <signal.h>
#include <stdio.h>
#include <fstream>
using namespace std;

//Functions
int createSocket();
struct sockaddr_in defineAddress(int PORTNUMBER);
void bindSocket(int anySocket, struct sockaddr_in anyAddress);
void listenForConnections(int anySocket);
int acceptConnections(int anySocket, struct sockaddr_in anyAddress);
//bool isBlacklist(int client_address);
void connectToServerGate(int anySocket, struct sockaddr_in anyAddress);

void registerUser();
bool isUser();
void checkDB();
void menuFunction();
void sighandler(int signum);
void transact_with_server(int sock);
void transact_with_client(int sock)



int createSocket()	// creates IPv4 socket
{
	int serv_fd = socket(AF_INET,SOCK_STREAM,0);
	if (serv_fd == -1)
	{
		perror("socket creation error");
		exit(1);
	}
	return serv_fd;
}

//initialize the socaddr_in structure
struct sockaddr_in defineAddress(int PORTNUMBER)
{
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORTNUMBER);
    return address;
}

//bind the socket with ip address and port
void bindSocket(int anySocket, struct sockaddr_in anyAddress) 
{
    if (bind(anySocket, (struct sockaddr *)&anyAddress, sizeof(anyAddress)) == -1)
    {
		perror("an error occured while binding");
		exit(1);
	}
}

void listenForConnections(int anySocket)	// listen for connections from given socket
{
    if (listen(anySocket, 3) == -1)
    {
		perror("an error occured while listning...!");
		exit(1);
	}
}

/*
bool isBlacklist(int client_fd)
{
    int black_list_ip;
    string client_address = client_fd.sin_addr.s_addr;
    ifstream file;
    file.open("/etc/edit/client_blacklist.txt");
    if (file.is_open())
    {
        while(getline (file, black_list_ip))
        {
            if (black_list_ip == client_address )
                return true;
            else
                return false;
        }
        file.close();
    }
    else 
        cout << "File cannot be read..!"<<endl;
}

*/

int acceptConnections(int serverSocket, struct sockaddr_in sock_addr_cli)
{
    socklen_t cli_len = sizeof(sock_addr_cli);
    int connect = accept(serverSocket, (struct sockaddr *)&sock_addr_cli, &cli_len);
    if (connect==-1)
    {
        perror(" accept error");
        exit(1);
    }
    return connect;
}


void connectToServerGate(int anySocket, struct sockaddr_in anyAddress)
{
    if (connect(anySocket, (struct sockaddr *)&anyAddress, sizeof(anyAddress)) == -1)
    {
		perror("connect error");
		exit(1);
	}
    else
    	cout<<" Getting connected ...!" << endl;
}


void menuFunction()
{
    cout << "Choose what to do:\n 1.Register\t 2.Login\t 3.Exit" << endl;
    int option;
    cin >> option;
    if (option == 1)
    {
        registerUser();
    }
    else if (option == 2)
    {
        checkDB();
    }
    else if (option == 3)
    {
        exit(0);
    }
    else
    {
        cout << "Wrong decision";
        exit(0);
    }
}

void registerUser()
{
	string username, password;
	cout << "Enter Username : "; cin>>username;
	cout << "Enter Password : "; cin>>password;
	
	ofstream file;
	file.open("/etc/edit/userlist.txt", ios::app); // Append mode    O_APPEND
	file << username << endl << password <<endl;
	file.close();


}


bool isUser()
{

    string username, password, usnm, pssd;
    cout << "Enter Username : "; cin>>username;
    cout << "\nEnter Password : "; cin>>password;
    
    ifstream file;
    file.open("/etc/edit/userlist.txt");
    if (file.is_open())
    {
        getline (file, usnm);
        getline (file, pssd);
        file.close();
    }
    else 
        cout << "File cannot be read..!"<<endl;

    if (usnm == username && pssd == password)
        return true;
    else
        return false;
}


void checkDB()
{
    int loginAttempt=0, totalAttempt=5;
    while(loginAttempt<5)
    {
        bool isavailable = isUser();
        if(!isavailable)
        {
            cout << "Invalid login attempt. Please try again.\n" << '\n';
            ++loginAttempt;
            cout << "Attempt Left : " << totalAttempt-loginAttempt << endl;
            if (loginAttempt == totalAttempt)
            {
                cout << "Too many login attempts.....! \nThank you for logging in.\n";
                exit(0);
            }
        }
        else
            cout<<"Welcome you.."<<endl;
            break;

    }
}

//client side
void transact_with_server(int sock)
{
    char ch[100], buf[1024];
    cout << "enter a string : "; cin >> ch;
    send(sock,ch,strlen(ch), 0);// writes a string to server

    int n =read(sock,buf,sizeof(buf));// reads the acknowledgement from server
    write(sock,buf,n);
    cout<<endl;
}

// server side
void transact_with_client(int sock)
{
    char ch[100];
    read(sock,ch,sizeof(ch));
    puts(ch);
    char greet[]={"hello "};
    strcat(greet,ch);
    send(sock,greet,strlen(greet), 0);  
    
}


//to avoid zombie process
void sighandler(int signum)
{
    cout << strsignal(signum) << endl;
    wait(NULL);
}


    