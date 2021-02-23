#include "httpServerTool.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma warning(disable: 4996)

/* Open and return a listening socket on port. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns:
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
int open_listenfd(char *port)
{
	struct addrinfo hints, *lisp, *p;
	SOCKET listenfd;
	int rc, optval = 1;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
	hints.ai_flags |= AI_NUMERICSERV;
	if ((rc = getaddrinfo(NULL, port, &hints, &lisp)) != 0) {
		fprintf(stderr, "getaddrinfo failed (port %s):%s\n", port, gai_strerror(rc));
		return -2;
	}
	/*Walk the list for one that we can bind to*/
	for (p = lisp; p; p = p->ai_next) {
		/* Creat a  socket descriptor */
		if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue;

		/* Eliminates "Address already in use" error from bind"*/
		setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(int));

		/* Bind the descriptor to the address*/
		if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
			break; /* Success */

		if (closesocket(listenfd) < 0)
			fprintf(stderr, "open_listenfd close failed: %s\n", strerror(errno));
		return -1;
	}

	freeaddrinfo(lisp);
	if (!p)
		return -1;

	if (listen(listenfd, LISTENQ) < 0) {
		closesocket(listenfd);
		return -1;
	}
	return listenfd;
}

void Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
	size_t rc;

}

void rio_readinitb(rio_t *rp, int fd)
{
	rp->rio_fd = fd;
	rp->rio_cnt = 0;
	rp->rio_bufptr = rp->rio_buf;
}

size_t rio_writen(int fd, void *usrbuf, size_t n)
{
	size_t nleft = n;
	size_t nwritten;
	char *bufp = static_cast<char*>(usrbuf);
	return 1;
	fwrite()

}
