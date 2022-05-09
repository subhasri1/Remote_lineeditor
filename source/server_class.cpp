#include "../include/functions.hpp"

void showCurrentDirectoyServer(int sock)
{
    char cwd[BUFFAR_SIZE];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        char msg[BUFFAR_SIZE] = {"Directory not found.."};
        send(sock, msg, BUFFAR_SIZE, 0);
    }
    else
    {
        send(sock, cwd, sizeof(cwd), 0);
    }	
}

void listFilesAndFolderServer(int sock)
{
	char buff[BUFFAR_SIZE];
    DIR *d;
    char ch[30];
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            strncpy(ch, dir->d_name, 30);
            ch[BUFFAR_SIZE] = '\0';
            strcat(buff, ch);
            strcat(buff, "\n");

        }
        closedir(d);
        send(sock, buff, BUFFAR_SIZE, 0);
    }
}

void changeDirectoryServer(char *command, int sock)
{
    char cwd[BUFFAR_SIZE];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        char msg[BUFFAR_SIZE] = {"Directory not found.."};
        send(sock, msg, BUFFAR_SIZE, 0);
    }
    chdir(command);
    getcwd(cwd, sizeof(cwd));
    send(sock, cwd, sizeof(cwd), 0);

}

void selectFileServer(char *command, int sock)
{
	struct stat sfile;
	string str = "cat ";
	if(system((str + command).c_str()) != 0)
	{
		int i=0;
		send(sock, &i, sizeof(int),0);
	}
	else
	{	
		str =str + command + " >temps.txt";
		const char *commandnew = str.c_str();
		system(commandnew);
		stat("temps.txt",&sfile);
		int size = sfile.st_size;
		send(sock, &size, sizeof(size),0);
		int filehandle = open("temps.txt", O_RDONLY);
		if(filehandle == -1)
		{
			perror("error in filehandle \n");
			exit(1);
		}
		sendfile(sock,filehandle,NULL,size);
		system("rm temps.txt");
	}
}

void printLineServer(char *filename, char *start, char *last, int sock)
{
	struct stat sfile;
	string str = "head -";		
	str =str+last+" "+filename+" | tail +"+start+" > line.txt" ;
	const char *commandnew = str.c_str();
	system(commandnew);
	stat("line.txt",&sfile);
	int size = sfile.st_size;
	send(sock, &size, sizeof(size),0);
	int filehandle = open("line.txt", O_RDONLY);
	if(filehandle == -1)
	{
		perror("error in filehandle \n");
		exit(1);
	}
	sendfile(sock,filehandle,NULL,size);
	system("rm line.txt");
}


void SERVER::transact_with_client(int sock)
{
	while(1)
	{	
		char buf[BUFFAR_SIZE];
		int n=recv(sock, buf, BUFFAR_SIZE, 0);
		buf[n]='\0';
      	char *command1=strtok(buf," ");
   		char *filename=strtok(NULL," ");
   		char *start=strtok(NULL," ");
   		char *last=strtok(NULL," ");
   		if(strcmp(command1, "pwd")==0)
		{
	    	showCurrentDirectoyServer(sock);
	    }
	    else if(strcmp(command1, "ls")==0)
		{
	    	listFilesAndFolderServer(sock);
	    }
	    else if(strcmp(command1, "cd")==0)
		{
		    changeDirectoryServer(filename, sock);
		}   
		else if(strcmp(command1, "select")==0)
		{
		    selectFileServer(filename, sock);
		}
		else if(strcmp(command1, "print")==0)
		{    
		    printLineServer(filename, start, last, sock);
		}
		if(strcmp(command1, "bye")==0)
		{    
		    int status = 1;
	  		send(sock, &status, sizeof(int), 0);
		}
	
		
	}
}

