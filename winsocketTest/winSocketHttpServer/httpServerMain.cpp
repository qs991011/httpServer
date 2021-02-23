#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include "httpServerTool.h"
#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

using namespace std;


void doit(int fd);
void read_requesthdrs(rio_t *rp);
//int parse_uri(char *uri, char *filename, char *cgiargs);
//void serve_static(int fd, char *filename, int filesize);
//void get_filetype(char* filename, char *cgiargs);
//void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

int main(int argc, char** argv)
{
	SOCKET listentfd, connfd;
	char hostname[MAXLEN], port[MAXLEN];
	socklen_t clientlen;
	sockaddr clientaddr;
	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}

	listentfd = open_listenfd(argv[1]);
	while (1)
	{
		clientlen = sizeof(clientaddr);
		connfd = accept(listentfd, &clientaddr, &clientlen);
		getnameinfo(&clientaddr, clientlen, hostname, MAXLEN, port, MAXLEN, 0);
		printf("Accept connection frm (%s,%s)\n", hostname, port);
		
		closesocket(connfd);
	}
	

	return 0;
}


/*
* doit handle one http request/response transaction
*/
void doit(int fd)
{
	int is_static;
	struct stat sbuf;
	char buf[MAXLEN], method[MAXLEN], uri[MAXLEN], version[MAXLEN];
	char filename[MAXLEN], cgiargs[MAXLEN];
	rio_t rio;
	/* Read request line and headers*/
	recv(fd, buf, sizeof(buf), 0);
	//rio_readinitb(&rio, fd);
	
	printf("%s", buf);
	sscanf(buf, "%s %s %s", method, uri, version);
	if (strcmp(method, "GET")) {
		clienterror(fd, method, "501", "Not Implemented", "Tiny does not implement this method");
		return;
	}

}

void read_requesthdrs(rio_t * rp)
{
	char buf[MAXLEN];
	rio_readinitb(rp, buf, MAXLEN);
	printf("%s", buf);
	while (strcmp(buf,"\r\n"))
	{
		rio_readinitb(rp, buf, MAXLEN);

	}
}

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
	char buf[MAXLEN];
	sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
}

