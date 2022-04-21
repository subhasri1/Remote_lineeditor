#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
using namespace std;

// client sends a single character ('a') to the server and it recieves ('b') from the server
int main()
{

    //socket creation
    int serv_fd = socket(AF_INET,SOCK_STREAM,0);
    if (serv_fd == -1)
    {
	    perror("socket creation error");
	    exit(1);
    }
    //initialize the socaddr_in structure
    struct sockaddr_in sock_addr_serv;// sockaddr_in for IPV4
    sock_addr_serv.sin_family = AF_INET;
    sock_addr_serv.sin_port = 8021;
    sock_addr_serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    // connecting to server
    socklen_t len = sizeof(sock_addr_serv);
    if (connect(serv_fd,(struct sockaddr *)&sock_addr_serv,len)==-1)
    {
	    perror("connect error");
	    exit(1);
    }
    cout << "in client client fd" << serv_fd << endl;
    cout << "in client port (50) " << sock_addr_serv.sin_port << endl;
    char ch = 'a';

    write(serv_fd,&ch,1);// writes 'a' to server
    read(serv_fd,&ch,1);// reads 'b' from server
    cout << "from server " << ch << endl;

    close(serv_fd);

    }









