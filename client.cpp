#include <iostream>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>         // Core BSD socket functions and data structures.
#include <netinet/in.h>         // AF_INET and AF_INET6 address families and their corresponding protocol families PF_INET and PF_INET6.
#include <arpa/inet.h>          // Functions for manipulating numeric IP addresses.
// #include <netinet/ip.h>

using namespace std;
#define PORT 8000;

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == -1)
    {
        perror("Socket Creation Failed...!\n");
        exit(1);
    }

    struct  sockaddr_in 
    {
        server.sin_family = AF_INET;
        server.sin_PORT = htons(PORT);
        server.sin_addr.s_addr = inet_addr("127.0.0.1");
    }server, client;
    
    int c = connect(client_fd, (struct sockaddr*)&servrer, sizeof(server));
    if(c == -1)
    {
        perror("Binding Failed...!");
        exit(1);
    }

    int size = sizeof(struct sockaddr*)&client;
    char snd[20], rcv[20];
    cout<<"connected to server...!"<<endl;
    while (1)
    {
        cout<<"Enter message to pass to server : ";
        cin>>snd;
        send(client_fd, snd, sizeof(snd), 0);
        if(strcmp(snd, "exit")==0)
            break;
        int r = recv(client_fd, rcv, sizeof(rcv), 0);
        rcv[r] = '\0';
        cout<<"MSG from client is : "<<rcv;
        
    }
    
}