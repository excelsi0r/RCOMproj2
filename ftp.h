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
	/*0*/"ERROR: Invalid error called",
	/*1*/"ERROR: Bad Arguments!\nUsage: download <url>",
	/*2*/"ERROR: Parsing invalid url",
	/*3*/"ERROR: Failed to retrieve address info",
	/*4*/"ERROR: Failed to retrieve IPV4 from server",
	/*5*/"ERROR: Failed to create sockets",
	/*6*/"ERROR: Failed to connect socket to server",
	/*7*/"ERROR: Failed to Read from Server",
	/*8*/"ERROR: Response from server wasn't a valid code",
	/*9*/"ERROR: Unexpected Response from Server",
	/*10*/"ERROR: Failed to Send command to Server",
	/*11*/"ERROR: Failed to create Command"
};

int parseURLelems(const char * argument, char * host, char * path, char * user, char * password);
int replacewithspaces(const char * resturl, char * spliturl);
int getResponseFromServer(int scoket, int expected, char * res);
int sendCommand(int socket, char * cmd);
int printerror(int num);
int printmsg(char * msg);

#endif
