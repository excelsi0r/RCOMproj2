#include "ftp.h"

int main(int argc, const char * argv[])
{

	//Checking if correct arguments
	if(argc != 3 || strcmp("download", argv[1]) != 0)
	{
		printerror(1);
		return 1;
	}
	
	
	//parsing elements
	char host[MAX_BUF];
	char path[MAX_BUF];
	char user[MAX_BUF];
	char pass[MAX_BUF];
	
	int error = parseURLelems(argv[2], host, path, user, pass);
	if(error != 0)
	{
		printerror(error);
		return error;
	}
	
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

int printerror(int errorno)
{
	int len = sizeof(error_msg)/sizeof(char *);
	
	if(errorno > len)
	{
		printf("%s", error_msg[0]);
		return 0;
	}

	printf("%s", error_msg[errorno]);
	return 0;
}




