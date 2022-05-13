#include "../include/commands.hpp"

/*********************************** socket connection *********************************************/

int Socket_Programming::create_socket()	// creates IPv4 socket
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
struct sockaddr_in Socket_Programming::define_address()
{
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    address.sin_port = htons(PORT_NUMBER);
    return address;
}

//bind the socket with ip address and port
void Socket_Programming::bind_socket(int anySocket, struct sockaddr_in anyAddress) 
{
    if (bind(anySocket, (struct sockaddr *)&anyAddress, sizeof(anyAddress)) == -1)
    {
		perror("an error occured while binding");
		exit(1);
	}
}

void Socket_Programming::listen_connections(int anySocket)	// listen for connections from given socket
{
    if (listen(anySocket, 3) == -1)
    {
		perror("an error occured while listning...!");
		exit(1);
	}
}

int Socket_Programming::accept_connections(int serverSocket, struct sockaddr_in sock_addr_cli)
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


void Socket_Programming::connect_to_server(int anySocket, struct sockaddr_in anyAddress)
{
    if (connect(anySocket, (struct sockaddr *)&anyAddress, sizeof(anyAddress)) == -1)
    {
		perror("connect error");
		exit(1);
	}
    else
	{
		cout<<"connection successful...";
		cout<<"\nplease enter exact Socket_Programming - \n";
		cout<<"======================================= help =========================================\n";
		cout<<"[pwd] 						to show present working directory"<<endl;
		cout<<"[cd path]					to change directory "<<endl;
		cout<<"[ls] 						to show present working directory "<<endl;
		cout<<"[select filename] 				to show file content "<<endl;
		cout<<"[edit filename linenum]				to edit chosen line "<<endl;
		cout<<"[print filename start_line end_line]		to print lines "<<endl;
		cout<<"[bye] 						to exit "<<endl;
		cout<<"======================================================================================\n";
	}
}

/*********************************** pwd command *********************************************/
void Show_Current_Directory::show_current_directoy_server_side(int sock)
{
	bzero(buf, BUFFER_SIZE);
    getcwd(cwd, sizeof(cwd));
    send(sock, cwd, sizeof(cwd), 0);	
}
void Show_Current_Directory::show_current_directoy_client_side(int sock)
{
	bzero(buf, BUFFER_SIZE);
    strcpy(buf, "pwd ");
    send(sock, buf, sizeof(buf), 0);
    recv(sock, buf, sizeof(buf), 0);
	cout<<buf<<endl;
}

/************************************ ls command **********************************************/

void List_Files_And_Folders::list_files_and_folder_server_side(int sock)
{
	bzero(buf, BUFFER_SIZE);
	DIR *d;
	struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            strncpy(ch, dir->d_name, FILE_SIZE);
            strcat(buf, ch);
            strcat(buf, "\n");
        }
        closedir(d);
    }
	send(sock, buf, sizeof(buf), 0);
}

void List_Files_And_Folders::list_files_and_folder_client_side(int sock)
{
	bzero(buf, BUFFER_SIZE);
    strcpy(buf, "ls "); 
    send(sock, buf, sizeof(buf), 0);
    recv(sock, buf, sizeof(buf), 0);
    cout<<buf;
}

/************************************ cd command *********************************************/

void Change_Directory::change_directory_server_side(char *command, int sock)
{
    if(chdir(command) == -1)
		send(sock, msg, sizeof(msg), 0);
	else
	{
    	getcwd(cwd, sizeof(cwd));
    	send(sock, cwd, sizeof(cwd), 0);
	}
}

void Change_Directory::change_directory_client_side(int sock)
{
	bzero(buf, BUFFER_SIZE);
    strcpy(buf, "cd ");
    cin>>buf+3;
    send(sock, buf, sizeof(buf), 0);   
    recv(sock, buf, sizeof(buf), 0);
    cout<<buf<<endl;

}

/*********************************** select command ******************************************/

void Select_File::select_file_server_side(char *filename, int sock)
{
	bzero(buf, BUFFER_SIZE);
	ifstream myfile (filename);
	if (!myfile.is_open())
		send(sock, msg, sizeof(msg), 0);
	else
	{	
		x=0;
		while ( getline (myfile,line) )
		{
			++x;
        	str = to_string(x);
        	strcat(buf, str.c_str());
        	strcat(buf, ". ");
        	strcat(buf, line.c_str());
        	strcat(buf, "\n");
		}
		myfile.close();
		send(sock, buf, sizeof(buf), 0);		
	}
}


void Select_File::select_file_client_side(int sock)
{
	bzero(buf, BUFFER_SIZE);
	strcpy(buf, "select ");
    cin>>buf+7;
    send(sock, buf, sizeof(buf), 0);   
    recv(sock, buf, sizeof(buf), 0);
    cout<<buf;
}

/************************************ print command *******************************************/

void Print_Line::print_line_server_side(char *filename, int sock)
{
	bzero(buf, sizeof(buf));
	recv(sock, &s, sizeof(int), 0);
	recv(sock, &l, sizeof(int), 0);
	ifstream file (filename);
	int y=0;
	while ( getline (file,line) )
	{
		++y;
	}
	file.close();
	ifstream myfile (filename);
	if(s<=0 || l>y)
		send(sock, path, sizeof(path), 0);
	else
	{
		if(l<s)
			send(sock, path, sizeof(path), 0);
		else if (myfile.is_open())
		{
			int x = 0;
			while ( getline (myfile,line2) )
			{
				++x;
				if((x>=s) && (x<=l))
				{
					strcat(buf, line2.c_str());
					strcat(buf, "\n");
				}
			}
			myfile.close();
			send(sock, buf, sizeof(buf), 0);
		}
		else
			send(sock, msg, sizeof(msg), 0);	
	}
}

void Print_Line::print_line_client_side(int sock)
{
	bzero(buf, BUFFER_SIZE);
	cin>>filename>>s>>l;
	strcpy(buf, "print ");					//(lpstat -p -d)  -> display printers
	strcat(buf, filename);					//(sudo lp -d <printer name> templine.txt)
	send(sock, buf, sizeof(buf), 0);		// 				or
	send(sock, &s, sizeof(int), 0);			//(sudo lpr -P <printer name> templine.txt)
	send(sock, &l, sizeof(int), 0);			//(lpoptions -d < printer name>)  -> set default printer
	recv(sock, buf, sizeof(buf), 0);		//to print no. of copies : (lp -n number <file name
	cout<<buf;										 
}

/************************************* edit command *******************************************/
void Edit_Line::edit_line_server_side(char *filename, int sock)
{
	recv(sock, &l, sizeof(int), 0);
	vector<string> content;
	ifstream myfile (filename);
	int y=0;
	while ( getline (myfile,line) )
	{
		++y;
	}
	myfile.close();
	ifstream file (filename);
	if(l<=0 || l>y)
		send(sock, line_msg, sizeof(line_msg), 0);
	else if (!file.is_open())
	{
		status =0;
		send(sock, &status, sizeof(int), 0);
		send(sock, msg, sizeof(msg), 0);
	}
	else
	{
		status =1;
		send(sock, &status, sizeof(int), 0);
		int x = 0;
		while( getline(file, line) )
		{
			++x;
			content.push_back(line);
			if((x==l))
			{
				strcat(buf, line.c_str());
				strcat(buf, "\n");
			}
		}
		send(sock, buf, sizeof(buf), 0);
		bzero(buf, BUFFER_SIZE);
		int n=recv(sock,buf,sizeof(buf),0);	// edited line is recieved from the client
		buf[n]='\0';
		content[l-1] = buf;
		bzero(buf, sizeof(buf));
		ofstream newfile (filename);
		for(auto it : content)
		{
			newfile << it << endl; // copy the vector content back to file
		}
		newfile.close();
		send(sock, suc_msg, SIZECMD, 0);
	}
	file.close();
	
}

void Edit_Line::edit_line_client_side(int sock)
{
	cin>>filename>>l;
	strcpy(buf, "edit ");
	strcat(buf,filename);
	send(sock, buf, sizeof(buf), 0);
	send(sock, &l, sizeof(int), 0);
	recv(sock, &status, sizeof(int), 0);
	if (!status)
	{
		recv(sock, buf, sizeof(buf), 0);
		cout<<buf;
	}
	else
	{
		recv(sock, buf, sizeof(buf), 0);
		cout<<buf<<endl;
		bzero(buf, sizeof(buf));
		cout<<"New Line : >> ";
		getline(cin>>ws,line);
		send(sock,line.c_str(), LINE_CONTENT, 0);	// edited line is sent to the server
		recv(sock, buf, sizeof(buf), 0);
		cout<<buf;
	}
}

/************************************* bye command *********************************************/

void Exit::good_bye_server_side(int sock)
{
	status = 1;
	send(sock, &status, sizeof(int), 0);
}

void Exit::good_bye_client_side(int sock)
{
	bzero(buf, BUFFER_SIZE);
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
