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

#define MAX_BUF 255

static const char * error_msg[] =
{
	"ERROR: Invalid error called\n",
	"ERROR: Bad Arguments!\nUsage: download <url>\n",
	"ERROR: Parsing invalid url\n"
};

int parseURLelems(const char * argument, char * host, char * path, char * user, char * password);
int replacewithspaces(const char * resturl, char * spliturl);
int printerror(int num);

#endif
