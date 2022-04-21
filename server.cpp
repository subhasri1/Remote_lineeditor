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
    system("clear");
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
    
    int b = bind(server_fd, (struct sockaddr*)&servrer, sizeof(server));
    if(b == -1)
    {
        perror("Binding Failed...!");
        exit(1);
    }
    
    listen(server_fd, 5);

    int size = sizeof(struct sockaddr*)&client;
    char snd[20], rcv[20];
    while (1)
    {
        int client_fd = accept(server_fd, (struct sockaddr*)&client, &size);
        if(client_fd == -1)
        {
            perror("accept error\n");
            exit(1);
        }
        cout<<"connection accepted"<<endl;
        for(;;)
        {
            int r = recv(client_fd, rcv, sizeof(rcv), 0);
            rcv[r] = '\0';
            cout<<"MSG from client is : "<<rcv;
            if(strcmp(rcv, "exit")==0)
                break;
            cout<<"Enter message to pass to client : ";
            cin>>snd;
            send(client_fd, snd, sizeof(snd), 0);
        }
    }
    
}