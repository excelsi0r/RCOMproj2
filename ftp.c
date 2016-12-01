#include "ftp.h"

int main(int argc, const char * argv[])
{

	//Checking if correct arguments
	if(argc != 2)
	{
		printerror(1);
		return 1;
	}
	
	
	//parsing elements
	char host[MAX_BUF];
	char path[MAX_BUF];
	char user[MAX_BUF];
	char pass[MAX_BUF];
	
	int error = parseURLelems(argv[1], host, path, user, pass);
	if(error != 0)
	{
		printerror(error);
		return error;
	}
	
	//Now with proper elements time to retrieve info prome server
	struct addrinfo *host_info, *host_info2;
	struct sockaddr_in * server_addr = NULL;
	
	//Getting information about the host provided
	if((error = getaddrinfo(host, "ftp", NULL, &host_info)) != 0)
	{
		printerror(3);
		return 3;
	}
	
	host_info2 = host_info;
	
	//Trying to get the proper IP address from the server
	//to connect to the socket in order to get the file
	//Server must be of type AF_INET (IPV4 type)
	//Searches for the first Server of type AF_INET
	//in the linked list of the same host
	while(host_info2 != NULL)
	{
		if(host_info2->ai_family == AF_INET)
		{
			server_addr = (struct sockaddr_in *)host_info2->ai_addr;
			break;	
		}
		
		host_info2 = host_info2->ai_next;
	}
	
	//In case the Server has no IPV4 (AF_INET) server type
	if(server_addr == NULL)
	{
		printerror(4);
		return 4;
	}
	
	//Creating sockets
	int socket1, socket2;
	
	//IPV4 and two-way connection-based byte stream
	if((socket1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printerror(5);
		return 5;
	}
	
	if((socket2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printerror(5);
		return 5;
	}
	
	//Establishing connection to the server on the first socket
	if(connect(socket1, (const struct sockaddr *)server_addr, sizeof(*server_addr)) != 0)
	{
		printerror(6);
		return 6;
	}
	
	//Upon connection it is time check if the server is ready
	char res[MAX_BUF];
	error = getResponseFromServer(socket1, SERVER_READY, res);
	if(error != 0)
	{
		printerror(error);
		
		if(error == 9)
			printf("Server Was not Ready, Response code: %s\n", res);
		
		return error;
	}
	
	printf("Server Ready \n");
	printmsg(res);	
	
	//Send user command
	char cmd[MAX_BUF];
	if(snprintf(cmd, MAX_BUF, "user %s\n", user) < 0)
	{
		printerror(11);
		return 11;
	}
	
	if((error = sendCommand(socket1, cmd)) != 0)
	{
		printerror(error);
		return error;
	}
	
	//Read user command response
	error = getResponseFromServer(socket1, USERNAME_OK, res);
	if(error != 0)
	{
		printerror(error);
		
		if(error == 9)
			printf("Failed to Enter Username, Response code: %s\n", res);
		
		return error;
	}
	
	printmsg(res);
	
	//Send password command
	if(snprintf(cmd, MAX_BUF, "pass %s\n", pass) < 0)
	{
		printerror(11);
		return 11;
	}
	
	if((error = sendCommand(socket1, cmd)) != 0)
	{
		printerror(error);
		return error;
	}
	
	//Read passsword command response
	error = getResponseFromServer(socket1, LOGIN_OK, res);
	if(error != 0)
	{
		printerror(error);
		
		if(error == 9)
			printf("Failed to Enter Password, Response code: %s\n", res);
		
		return error;
	}
	
	printmsg(res);
	
	//Send Passive mode Command
	if((error = sendCommand(socket1, "pasv\n")) != 0)
	{
		printerror(error);
		return error;
	}
	
	//Read the Passive Mode command response
	error = getResponseFromServer(socket1, PASS_MODE, res);
	if(error != 0)
	{
		printerror(error);
		
		if(error == 9)
			printf("Failed to Enter Passive Mode, Response code: %s\n", res);
		
		return error;
	}
	
	printmsg(res);
	
	
	
	

	return 0;
}

int parseURLelems(const char * argument, char * host, char * path, char * user, char * password)
{			
	//This parser processes two possive formats.
	//First format is ftp://<user>:<password>@<host>/<url-path>
	
	//Check if 'ftp://' exists, first step, puts in
	//resturl the url without 'ftp://' wheter it existed or
	//not in argument
	char resturl[MAX_BUF];	
	if(sscanf(argument, "ftp://%s", resturl) < 1)
	{
		strncpy(resturl, argument, strlen(argument) + 1);		
	}
	
		
	//For an easy parsing, the special characters 
	//that separate the user, password, host and path
	char spliturl[MAX_BUF];
	if(replacewithspaces(resturl, spliturl) != 0)
	{
		return 2;
	}
	
	
	//Time to get all four arguments from the new url
	//char spliturl[MAX_BUF];
	if(sscanf(spliturl, "%s %s %s %s", user, password, host, path) < 4)
	{
		//In case of public ftp with no user or password
		strncpy(user, "ftp", strlen("ftp") + 1);
		strncpy(password, "ftp", strlen("ftp") + 1);
		
		//Proper parse host and path again
		if(sscanf(spliturl, "%s %s", host, path) < 2)
			return 2;
	}

	return 0;
}

int replacewithspaces(const char * resturl, char * spliturl)
{
	int length = strlen(resturl);
	memcpy(spliturl, resturl, length + 1);
	
	int twopoints = 0;
	int arroba = 0;
	int i = 0;
	
	for(i = 0; i < length; i++)
	{
		if(spliturl[i] == ':')
		{
			spliturl[i] = ' ';
			twopoints = 1;
		}
		else if(spliturl[i] == '@')
		{
			if(!twopoints) //has password but not user
				return -1;
			spliturl[i] = ' ';
			arroba = 1;		
		}
		else if(spliturl[i] == '/')
		{
			if(twopoints && !arroba) //is passing host and path with user and no pass
				return -1;
			spliturl[i] = ' ';//first ocurrence separates host and path
			return 0;//has proper user, pass, host and path
		}
	}
	return -1;
}

int getResponseFromServer(int socket, int expected, char * res)
{
	int code;
	ssize_t resSize;
	
	if((resSize = read(socket, res, MAX_BUF - 1)) < 0)
	{
		return 7;
	}
	
	res[resSize] = 0;
	
	if(sscanf(res, "%d", &code) < 1)
	{
		return 8;
	}
	
	if(code != expected)
	{
		return 9;
	}
	
	return 0;	
}

int sendCommand(int socket, char * cmd)
{
	ssize_t bytesWritten;
	int bytesToWrite = strlen(cmd);
	
	if((bytesWritten = write(socket, cmd, bytesToWrite)) < 0)
	{
		return 10;
	}
	
	if(bytesWritten != bytesToWrite)
	{
		return 10;
	}
	
	return 0;
}

int printerror(int errorno)
{
	int len = sizeof(error_msg)/sizeof(char *);
	
	if(errorno > len)
	{
		printf("%s\n", error_msg[0]);
		return 0;
	}

	printf("%s\n", error_msg[errorno]);
	return 0;
}

int printmsg(char * msg)
{
	char greet[MAX_BUF];
	int n;
	
	sscanf(msg, "%d %[^\n]s", &n, greet);
	
	printf("Code: %d | %s\n",n, greet);
	return 0;
}




