#ifndef HTTPSERVERTOOL_H
#define HTTPSERVERTOOL_H
#include <iostream>

#define RIO_BUFSIZE 8192
#define MAXLEN 8192 /* Max text line length */
#define MAXBUFF 8192 /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */

typedef struct {
	int rio_fd;                /* Descriptor for this internal buf */
	int rio_cnt;               /* Unread bytes in internal buf */
	char *rio_bufptr;          /* Next unread byte in internal buf */
	char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;

int open_listenfd(char *port);
void rio_readinitb(rio_t *rp, int fd);
void Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
#endif // !HTTPSERVERTOOL_H



