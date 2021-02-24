#include <iostream>
#include <WinSock2.h>
#include <string.h>

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
#define PORT 80   
#define BUFFLEN 1024
#define MAXLEN 8192 /* Max text line length */
#define MAXBUF 8192 /* Max I/O buffer size */

using namespace std;
void doit(SOCKET fd);
int parse_uri(std::string  uri, std::string& filename, std::string& cgiargs);
void serve_dynamic(int fd, char *filename, char *cgiargs);
int main(int argc, char **argv)
{
	WSADATA wsaData;
	//1 ��������ʼ��winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		return false;
	//2 �����׽���
	SOCKET sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET)
	{
		WSACleanup();//WSACleanup��ֹ�����̵߳�Windows�׽��ֲ���
		return false;
	}
	//3 ׼��ͨ�ŵ�ַ
	SOCKADDR_IN addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(PORT);//u_short������ת��ΪTCP / IP�����ֽ�˳��
	addrServer.sin_addr.s_addr = INADDR_ANY; // ������õ�ַ

	//4 �󶨵�ַ��socket
	if (SOCKET_ERROR == bind(sServer, (const sockaddr*)&addrServer, sizeof(SOCKADDR_IN))) {
		closesocket(sServer);
		WSACleanup();
		return false;
	}
	//5 ����
	if (SOCKET_ERROR == listen(sServer, SOMAXCONN))
	{
		closesocket(sServer);
		WSACleanup();
	}
	while (1)
	{
		//6 �ȴ��ͻ�������
		sockaddr_in addrClient;
		int addrClientLen = sizeof(addrClient);
		std::cout << "�����������ɹ�,�ȴ��ͻ��������С�����" << std::endl;
		SOCKET sClient = accept(sServer, (sockaddr *)&addrClient, &addrClientLen);
		if (INVALID_SOCKET == sClient) {
			std::cout << WSAGetLastError() << std::endl;
			closesocket(sServer);
			closesocket(sClient);
			WSACleanup();
			return false;
		}
		std::cout << "�ͻ���(" << inet_ntoa(addrClient.sin_addr) << ":" << addrClient.sin_port << "):" << std::endl;
		doit(sClient);
		closesocket(sClient);
	}
}

void doit(SOCKET fd)
{
	int is_static; //�Ƿ��Ǿ�̬����
	// 7 ��������
	char recvBuf[BUFFLEN], method[MAXBUF], uri[MAXBUF], version[MAXBUF];
	std::string filename, cgiargs;

	ZeroMemory(recvBuf, sizeof(recvBuf));
	if (SOCKET_ERROR == recv(fd, recvBuf, sizeof(recvBuf), 0))
	{
		return ;
	}
	std::cout << "�յ���Ϣ: " << recvBuf << std::endl;

	sscanf(recvBuf, "%s %s %s", method, uri, version);

	if (strcmp(method, "GET")) {                     //line:netp:doit:beginrequesterr
		/*clienterror(fd, method, "501", "Not Implemented",
			"Tiny does not implement this method");*/
		//return;
	}

	is_static = parse_uri(uri, filename, cgiargs);

	//8 ��������
	char sendBuf[BUFFLEN] = "��ÿͻ���";
	if (SOCKET_ERROR == send(fd, sendBuf, sizeof(sendBuf), 0))
	{
		return ;
	}
}
/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
 /* $begin parse_uri */
//int parse_uri(char * uri, char * filename, char * cgiargs)
int parse_uri(std::string  uri, std::string& filename, std::string& cgiargs)
{
	char *ptr;

	if (!strstr(uri.c_str(), "cgi-bin")) {  /* Static content */ //line:netp:parseuri:isstatic
		//strcpy(cgiargs, "");                             //line:netp:parseuri:clearcgi
		cgiargs = "";
		filename = ",";
		//strcpy(filename, ".");                           //line:netp:parseuri:beginconvert1
		filename.append(uri);
		//strcat(filename, uri);                           //line:netp:parseuri:endconvert1
		if (uri[uri.length() - 1] == '/')                   //line:netp:parseuri:slashcheck
			//strcat(filename, "home.html");               //line:netp:parseuri:appenddefault
			filename.append("home.html");
		return 1;
	}
	else {  /* Dynamic content */                        //line:netp:parseuri:isdynamic
		//ptr = index(uri, '?');                           //line:netp:parseuri:beginextract
		size_t lo = uri.find_first_of('?');
		if (lo != std::string::npos) {
			cgiargs = uri.substr(lo + 1);
		}
		else {
			cgiargs = "";
		}
	
		/*if (ptr) {
			strcpy(cgiargs, ptr + 1);
			*ptr = '\0';
		}
		else
			strcpy(cgiargs, "");  */                       //line:netp:parseuri:endextract
		//strcpy(filename, ".");                           //line:netp:parseuri:beginconvert2
		//strcat(filename, uri);                           //line:netp:parseuri:endconvert2
		filename = ".";
		filename.append(uri);
		return 0;
	}
}

void serve_dynamic(int fd, char *filename, char *cgiargs)
{
	char buf[BUFFLEN], *emptylist[] = { NULL };

	/* Return first part of HTTP response */
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	//8 ��������
	if (SOCKET_ERROR == send(fd, buf, sizeof(buf), 0))
		return;
	sprintf(buf, "Server: Tiny Web Server\r\n");
	if (SOCKET_ERROR == send(fd, buf, sizeof(buf), 0))
		return;

	
}
