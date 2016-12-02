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

#define SERVER_READY			220
#define USERNAME_OK			331
#define LOGIN_OK			230
#define PASS_MODE			227
#define FILE_STATUS_OK			150
#define FILE_SEND_OK			226
#define GOOD_BYE			221
#define USR_GRP				0660

static const char * error_msg[] =
{
	/*00*/"ERROR: Invalid error called",
	/*01*/"ERROR: Bad Arguments!\nUsage: download <url>",
	/*02*/"ERROR: Parsing invalid url",
	/*03*/"ERROR: Failed to retrieve address info",
	/*04*/"ERROR: Failed to retrieve IPV4 from server",
	
	/*05*/"ERROR: Failed to create sockets",
	/*06*/"ERROR: Failed to connect socket to server",
	/*07*/"ERROR: Failed to Read from Server",
	/*08*/"ERROR: Response from server wasn't a valid code",
	/*09*/"ERROR: Unexpected Response from Server",
	
	/*10*/"ERROR: Failed to Send command to Server",
	/*11*/"ERROR: Failed to create Command to send",
	/*12*/"ERROR: Failed to Parse the new Passive Mode connection address",
	/*13*/"ERROR: Failed to create File",
	/*14*/"ERROR: Failed to Write to new File",
	
	/*15*/"ERROR: Failed to Read from File from Server",
	/*16*/"ERROR: Failed to Close the File Created",
	/*17*/"ERROR: Failed to Close Socket"
};

int parseURLelems(const char * argument, char * host, char * path, char * user, char * password);
int replacewithspaces(const char * resturl, char * spliturl);
int getResponseFromServer(int scoket, int expected, char * res);
int sendCommand(int socket, char * cmd);
int parsePassiveModeParams(const char *res, int *connectionAddress);
int readFile(int socket, int fd);
int printerror(int num);
int printmsg(char * msg);

#endif
