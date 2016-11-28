#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char * argv[])
{
	//Checking if correct arguments
	if(argc != 3 || strcmp("download", argv[1]) != 0)
	{
		printf("ERROR: Bad Arguments!\n");
		printf("Usage: download <url>\n");
		return -1;
	}
}
