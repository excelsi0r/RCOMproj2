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
	return 0;
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




