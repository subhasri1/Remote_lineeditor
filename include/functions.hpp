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

#define BUFFAR_SIZE 100
#define COMMAND_SIZE 15
#define FILE_SIZE 30
#define PORT_NUMBER 8021


/*=================================== This is from client side ===========================================*/
class CLIENT
{
	public:
		void transact_with_server(int sock);
};



/*================================= This is from server side =============================================*/

class SERVER
{
	public:
		void transact_with_client(int sock);
};