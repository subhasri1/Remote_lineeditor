#include "../include/commands.hpp"

void Show_Current_Directory::show_current_directoy_server(int sock)
{
	cout<<"pwd in server\n";
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        send(sock, msg, BUFFER_SIZE, 0);
    }
    else
    {
		cout<<cwd<<endl;
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

/**********************************************************************************************/

void List_Files_And_Folders::list_files_and_folder_server(int sock)
{
	DIR *d;
	struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            strncpy(ch, dir->d_name, 30);
            ch[BUFFER_SIZE] = '\0';
            strcat(buf, ch);
            strcat(buf, "\n");
        }
        closedir(d);
        send(sock, buf, BUFFER_SIZE, 0);
    }
}

void List_Files_And_Folders::list_files_and_folder_client(int sock)
{
    strcpy(buf, "ls "); 
    send(sock, buf, sizeof(buf), 0);
    recv(sock, buf, sizeof(buf), 0);
    cout<<endl<<buf;
}

/*********************************************************************************************/

void Change_Directory::change_directory_server(char *command, int sock)
{
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        send(sock, msg, BUFFER_SIZE, 0);
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

/*********************************************************************************************/

void Select_File::select_file_server(char *command, int sock)
{
	string str = "cat ";
	if(system((str + command).c_str()) != 0)
	{
		send(sock, &i, sizeof(int),0);
	}
	else
	{	
		str =str + command + " >temps.txt";
		const char *commandnew = str.c_str();
		system(commandnew);
		stat("temps.txt",&sfile);
		
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

/*********************************************************************************************/

void Print_Line::print_line_server(char *filename, char *start, char *last, int sock)
{
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
	if(size == -1)
	{
		perror("No lines found\n\n");
		exit(1);
	}
	char *f = new char(size);
	recv(sock, f, size, 0);
	int filehandle = creat("templine.txt", O_RDONLY);
	write(filehandle, f, size);							//(lpstat -p -d)  -> display printers
	close(filehandle);		
	cout<<endl;											//(sudo lp -d <printer name> templine.txt) 
	system("sudo cat templine.txt"); 
	cout<<endl;										// 				or
	delete f;									//(sudo lpr -P <printer name> templine.txt)	
	system("sudo rm templine.txt");				//(lpoptions -d < printer name>)  -> set default printer
									//to print no. of copies : (lp -n number <file name 
}

/*********************************************************************************************/


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