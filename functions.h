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
void userRegistration();
void login();
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

bool isBlacklist(int client_ip)
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
        else (usnm == username && pssd == password)
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
            else (usnm == username && pssd == password)
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

    