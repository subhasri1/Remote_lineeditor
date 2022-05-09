#include "headers.hpp"

/*********************************** socket connection *********************************************/

int Commands::create_socket()	// creates IPv4 socket
{
	int serv_fd = socket(AF_INET,SOCK_STREAM,0);
	if (serv_fd == -1)
	{
		foutlog<<"[ERROR] : Socket creation error "<<endl;
		perror("socket creation error");
		exit(1);
	}
	return serv_fd;
}

//initialize the socaddr_in structure
struct sockaddr_in Commands::define_address()
{
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    address.sin_port = htons(PORT_NUMBER);
    return address;
}

//bind the socket with ip address and port
void Commands::bind_socket(int anySocket, struct sockaddr_in anyAddress) 
{
    if (bind(anySocket, (struct sockaddr *)&anyAddress, sizeof(anyAddress)) == -1)
    {
    		foutlog<<"[ERROR] : Socket Binding error "<<endl;
		perror("an error occured while binding");
		exit(1);
	}
}

void Commands::listen_connections(int anySocket)	// listen for connections from given socket
{
    if (listen(anySocket, 3) == -1)
    {
    		foutlog<<"[ERROR] : ERROR in Server Listening "<<endl;
		perror("an error occured while listning...!");
		exit(1);
	}
}

int Commands::accept_connections(int serverSocket, struct sockaddr_in sock_addr_cli)
{
    socklen_t cli_len = sizeof(sock_addr_cli);
    int connect = accept(serverSocket, (struct sockaddr *)&sock_addr_cli, &cli_len);
    if (connect==-1)
    {
        foutlog<<"[ERROR] : Server Acceptance error "<<endl;
        perror(" accept error");
        exit(1);
    }
    return connect;
}


void Commands::connect_to_server(int anySocket, struct sockaddr_in anyAddress)
{
    if (connect(anySocket, (struct sockaddr *)&anyAddress, sizeof(anyAddress)) == -1)
    {
		perror("connect error");
		exit(1);
	}
    else
	{
		cout<<"connection successful...";
		cout<<"\nplease enter exact commands - \n";
		cout<<"======================================= help =========================================\n";
		cout<<"[pwd] 						to show present working directory"<<endl;
		cout<<"[cd path]					to change directory "<<endl;
		cout<<"[ls] 						to show present working directory "<<endl;
		cout<<"[select filname] 				to show file content "<<endl;
		cout<<"[print filename start line no. end line no.]	to print lines "<<endl;
		cout<<"[bye] 						to exit "<<endl;
		cout<<"======================================================================================\n";
	}
}


/*********************************** pwd command *********************************************/
void Show_Current_Directory::show_current_directoy_server(int sock)
{
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        send(sock, msg, BUFFER_SIZE, 0);
    }
    else
    {
        send(sock, cwd, sizeof(cwd), 0);
    }	
}
void Show_Current_Directory::show_current_directoy_client(int sock)
{
    strcpy(buf, "pwd ");
    send(sock, buf, sizeof(buf), 0);
    recv(sock, buf, sizeof(buf), 0);
	cout<<endl<<buf<<endl;
}

/************************************ ls command **********************************************/

void List_Files_And_Folders::list_files_and_folder_server(int sock)
{
	bzero(buf, sizeof(buf));
	DIR *d;
	struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            strncpy(ch, dir->d_name, FILE_SIZE);
            //ch[BUFFER_SIZE] = '\0';
            strcat(buf, ch);
            strcat(buf, "\n");
        }
        closedir(d);
    }
	send(sock, buf, sizeof(buf), 0);
}

void List_Files_And_Folders::list_files_and_folder_client(int sock)
{
    strcpy(buf, "ls "); 
    send(sock, buf, sizeof(buf), 0);
    recv(sock, buf, sizeof(buf), 0);
    cout<<buf;
}

/************************************ cd command *********************************************/

void Change_Directory::change_directory_server(char *command, int sock)
{
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        send(sock, msg, sizeof(msg), 0);
    }
    chdir(command);
    getcwd(cwd, sizeof(cwd));
    send(sock, cwd, sizeof(cwd), 0);
}

void Change_Directory::change_directory_client(int sock)
{
    strcpy(buf, "cd ");
    cin>>buf+3;
    send(sock, buf, sizeof(buf), 0);   
    recv(sock, buf, sizeof(buf), 0);
    cout<<endl<<buf<<endl;

}

/*********************************** select command ******************************************/

void Select_File::select_file_server(char *command, int sock)
{
	string str = "cat ";
	str =str + command + " >temps.txt";
	if(system(str.c_str()) != 0)
	{
		send(sock, &i, sizeof(int),0);
	}
	else
	{
		stat("temps.txt",&sfile);
		size = sfile.st_size;
		send(sock, &size, sizeof(size),0);
		int filehandle = open("temps.txt", O_RDONLY);
		if(filehandle == -1)
		{
			perror("error in filehandle \n");
			exit(1);
		}
		sendfile(sock, filehandle, NULL,size);
		system("rm temps.txt");
	}
}


void Select_File::select_file_client(int sock)
{
	strcpy(buf, "select ");
	cin>>filename;
	strcat(buf, filename);
	send(sock, buf, sizeof(buf), 0);
	recv(sock, &size, sizeof(size), 0);
	if(size == 0)
	{
		cout<<"No such file on the remote directory\n";
	}
	else{
		char *f = new char(size);
		recv(sock, f, size, 0);
		int filehandle = creat("temp.txt", O_RDONLY);
		write(filehandle, f, size);
		close(filehandle);
		cout<<endl;
		system("sudo cat -n temp.txt");
		cout<<endl;	
		delete f;
		system("sudo rm temp.txt");
	}
}

/************************************ print command *******************************************/

void Print_Line::print_line_server(char *filename, char *start, char *last, int sock)
{
	string str = "head -";		
	str =str+last+" "+filename+" | tail +"+start+" > line.txt" ;
	const char *commandnew = str.c_str();
	if(system(commandnew) != 0)
	{
		send(sock, &i, sizeof(int),0);
	}
	stat("line.txt", &sfile);
	int size = sfile.st_size;
	send(sock, &size, sizeof(size),0);
	int filehandle = open("line.txt", O_RDONLY);
	if(filehandle == -1)
	{
		perror("error in filehandle \n");
		exit(1);
	}
	sendfile(sock, filehandle, NULL, size);
	system("rm line.txt");
}

void Print_Line::print_line_client(int sock)
{
	cin>>filename>>s>>l;
	strcpy(buf, "print ");
	strcat(buf, filename);
	strcat(buf, " ");
	strcat(buf, s);
	strcat(buf, " ");
	strcat(buf, l);
	send(sock, buf, sizeof(buf), 0);
	recv(sock, &size, sizeof(size), 0);
	if(size == 0)
	{
		cout<<"No such file on the remote directory\n";
	}
	char *f = new char(size);
	recv(sock, f, size, 0);
	int filehandle = creat("templine.txt", O_RDONLY);
	write(filehandle, f, size);							//(lpstat -p -d)  -> display printers
	close(filehandle);		
	cout<<endl;											//(sudo lp -d <printer name> templine.txt) 
	system("sudo cat templine.txt"); 
									// 				or
	delete f;									//(sudo lpr -P <printer name> templine.txt)	
	system("sudo rm templine.txt");				//(lpoptions -d < printer name>)  -> set default printer
									//to print no. of copies : (lp -n number <file name 
}

/************************************* bye command *********************************************/


void Exit::good_bye_server(int sock)
{
	status = 1;
	send(sock, &status, sizeof(int), 0);
}

void Exit::good_bye_client(int sock)
{
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

