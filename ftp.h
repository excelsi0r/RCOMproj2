#ifndef FTP_H
#define TFP_H

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

#define MAX_BUF 256

static const char * error_msg[] =
{
	"ERROR: Invalid error called\n",
	"ERROR: Bad Arguments!\nUsage: download <url>\n"	
};

int parseURLelems(char * argument, char * host, char * path, char * user, char * password);
int printerror(int num);

#endif
