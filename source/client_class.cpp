#include "../include/functions.hpp"


void showCurrentDirectoyClient(int sock)
{
	char buf[BUFFAR_SIZE];
    strcpy(buf, "pwd ");
    send(sock, buf, sizeof(buf), 0);
    bzero(buf, sizeof(buf));
    recv(sock, buf, sizeof(buf), 0);
    cout<<buf;
}
void listFilesAndFolderClient(int sock)
{
	char buf[BUFFAR_SIZE];
    strcpy(buf, "ls "); 
    send(sock, buf, sizeof(buf), 0);
    bzero(buf, sizeof(buf));
    recv(sock, buf, sizeof(buf), 0);
    cout<<buf;
}
void changeDirectoryClient(int sock)
{
	char buf[BUFFAR_SIZE];
    strcpy(buf, "cd ");
    cin>>buf+3;
    send(sock, buf, sizeof(buf), 0);
    bzero(buf, sizeof(buf));    
    recv(sock, buf, sizeof(buf), 0);
    cout<<buf;

}
void selectFileClient(int sock)
{
	char buf[BUFFAR_SIZE], filename[BUFFAR_SIZE];
	int size;
	strcpy(buf, "select ");
	cin>>filename;
	strcat(buf, filename);
	send(sock, buf, sizeof(buf), 0);
	bzero(buf, sizeof(buf));
	recv(sock, &size, sizeof(size), 0);
	if(size == 0)
	{
		cout<<"No such file on the remote directory\n";
	}
	else
	{
		char *f = new char(size);
		recv(sock, f, size, 0);
		int filehandle = creat("temp.txt", O_RDONLY);
		write(filehandle, f, size);
		close(filehandle);
		system("sudo cat -n temp.txt");
		delete f;
		system("sudo rm temp.txt");
	}
	
}
void printLineClient(int sock)
{
	char buf[BUFFAR_SIZE], filename[FILE_SIZE], s[3], l[3];
	int size;
	cin>>filename>>s>>l;
	strcpy(buf, "print ");
	strcat(buf, filename);
	strcat(buf, " ");
	strcat(buf, s);
	strcat(buf, " ");
	strcat(buf, l);
	send(sock, buf, sizeof(buf), 0);
	bzero(buf, sizeof(buf));
	recv(sock, &size, sizeof(size), 0);
	if(size == -1)
	{
		perror("No lines found\n\n");
		exit(1);
	}
	char *f = new char(size);
	recv(sock, f, size, 0);
	int filehandle = creat("templine.txt", O_RDONLY);
	write(filehandle, f, size);							//(lpstat -p -d)  -> display printers
	close(filehandle);								//(sudo lp -d <printer name> templine.txt) 
	system("sudo cat templine.txt"); 				// 				or
	delete f;									//(sudo lpr -P <printer name> templine.txt)	
	system("sudo rm templine.txt");				//(lpoptions -d < printer name>)  -> set default printer
												//to print no. of copies : (lp -n number <file name 
}

void goodByeClient(int sock)
{
	char buf[BUFFAR_SIZE];
	int status;
    strcpy(buf, "bye ");
    send(sock, buf, sizeof(buf), 0);
    recv(sock, &status, sizeof(int), 0);
    if(status)
    {
        cout<<"Termination Successful...\nThank you...\n";
        close(sock);
        exit(0);
    }
}



void CLIENT::transact_with_server(int sock)
{
	while(1)
	{
		string choice;
      	cout<<"\nRemoteLineEditor~$ ";
      	cin>>choice;
      	if(choice == "pwd")
      	{
      		showCurrentDirectoyClient(sock);
      	}
		else if(choice == "ls")
      	{
			listFilesAndFolderClient(sock);
		}
		else if(choice == "cd")
      	{
			changeDirectoryClient(sock);
		}
		else if(choice == "select")
      	{
			selectFileClient(sock);
		}
		else if(choice == "print")
      	{
			printLineClient(sock);
		}
		else if(choice == "bye")
      	{
			goodByeClient(sock);
		}
		else
		{
			cout<<"command not found..."<<endl;
		}

	}


}

