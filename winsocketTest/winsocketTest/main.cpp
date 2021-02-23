#include <iostream>
#include <WinSock2.h>
#include "yyjson.h"
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
#define PORT 9999   
#define BUFFLEN 1024
#define MAXLEN 8192 /* Max text line length */
#define MAXBUF 8192 /* Max I/O buffer size */



int main(int argc, char **argv)
{
	

	WSADATA wsaData;
	//1 启动并初始化winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		return false;
	//2 创建套接字
	SOCKET sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET)
	{
		WSACleanup();//WSACleanup终止所有线程的Windows套接字操作
		return false;
	}
	//3 准备通信地址
	SOCKADDR_IN addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(PORT);//u_short从主机转换为TCP / IP网络字节顺序
	addrServer.sin_addr.s_addr = INADDR_ANY; // 任意可用地址

	//4 绑定地址与socket
	if (SOCKET_ERROR == bind(sServer, (const sockaddr*)&addrServer, sizeof(SOCKADDR_IN))) {
		closesocket(sServer);
		WSACleanup();
		return false;
	}
	//5 监听
	if (SOCKET_ERROR == listen(sServer, SOMAXCONN))
	{
		closesocket(sServer);
		WSACleanup();
	}
	//6 等待客户端连接
	sockaddr_in addrClient;
	int addrClientLen = sizeof(addrClient);
	std::cout << "服务器启动成功,等待客户端连接中。。。" << std::endl;
	SOCKET sClient = accept(sServer, (sockaddr *)&addrClient, &addrClientLen);
	if (INVALID_SOCKET == sClient) {
		std::cout << WSAGetLastError() << std::endl;
		closesocket(sServer);
		closesocket(sClient);
		WSACleanup();
		return false;
	}

	// 7 接收数据
	char recvBuf[BUFFLEN];
	ZeroMemory(recvBuf, sizeof(recvBuf));
	if (SOCKET_ERROR == recv(sClient, recvBuf, sizeof(recvBuf), 0))
	{
		closesocket(sServer);
		closesocket(sClient);
		WSACleanup();
		return false;
	}

	std::cout << "客户端(" << inet_ntoa(addrClient.sin_addr) << ":" << addrClient.sin_port << "):" << recvBuf << std::endl;
	//8 发送数据
	char sendBuf[BUFFLEN] = "你好客户端";
	if (SOCKET_ERROR == send(sClient, sendBuf, sizeof(sendBuf), 0))
	{
		closesocket(sServer);
		closesocket(sClient);
		WSACleanup();
		return false;
	}
	system("pause");
	system("pause");
	return true;
	std::cout << "winsocket test" << std::endl;
}