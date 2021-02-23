
#include <iostream>
#include <WinSock2.h>

#include "yyjson.h"

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable: 4996)
using namespace std;
#define BufLen 1024
#define PORT 9999
typedef size_t      usize;
#define USIZE_MAX       ((usize)(~(usize)0))

void getIp(char* szHostaddress);

template<typename... Ts> void afunc(Ts... args) {
	const int size = sizeof...(args) + 2;
	int res[size] = { 1, args..., 2 };
	int dummy[sizeof...(Ts)] = { (std::cout << args, args)... };
}

//void test_yyjson() {
//	const char *json = "{\"name\":\"Mash\",\"star\":4,\"hits\":[2,2,1,3]}";
//	yyjson_doc *doc = yyjson_read(json, strlen(json), 0);
//	yyjson_val *root = yyjson_doc_get_root(doc);
//
//	yyjson_val *name = yyjson_obj_get(root, "name");
//	printf("name: %s\n", yyjson_get_str(name));
//	printf("name length:%d\n", (int)yyjson_get_len(name));
//
//	yyjson_val *star = yyjson_obj_get(root, "star");
//	printf("star: %d\n", (int)yyjson_get_int(star));
//
//	yyjson_val *hits = yyjson_obj_get(root, "hits");
//	size_t idx, max;
//	yyjson_val *hit;
//	yyjson_arr_foreach(hits, idx, max, hit) {
//		printf("hit%d: %d\n", (int)idx, (int)yyjson_get_int(hit));
//	}
//
//	yyjson_doc_free(doc);
//}
int main() {
	//afunc(1, 2, 3);

	cout << USIZE_MAX << endl;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		return false;
	}
	//2 创建套接字
	SOCKET sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		closesocket(sHost);
		WSACleanup();
		return false;
	}
	//3 准备通信地址
	char szHostaddress[200];
	getIp(szHostaddress);
	SOCKADDR_IN addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(PORT);
	addrServer.sin_addr.s_addr = inet_addr(szHostaddress);
	//4 连接服务器
	if (SOCKET_ERROR == connect(sHost, (const sockaddr*)&addrServer, sizeof(addrServer)))
	{
		closesocket(sHost);
		WSACleanup();
		return false;
	}
	//5 发送数据
	char sendBuf[BufLen] = "你好服务器";
	if (SOCKET_ERROR == send(sHost, sendBuf, sizeof(sendBuf), 0))
	{
		closesocket(sHost);
		WSACleanup();
		return false;
	}

	cout << "客户端发送消息成功！" << endl;
	char recvbuf[BufLen];
	ZeroMemory(recvbuf, sizeof(recvbuf));
	if (SOCKET_ERROR == recv(sHost, recvbuf, sizeof(recvbuf), 0))
	{
		closesocket(sHost);
		WSACleanup();
		return FALSE;
	}

	cout << "服务器发来的消息：" << recvbuf << endl;
	system("pause");
}

void getIp(char* szHostaddress) {
	char szHostname[100];
	if (gethostname(szHostname, sizeof(szHostname)) != SOCKET_ERROR)
	{
		HOSTENT *pHostEnt = gethostbyname(szHostname);
		if (pHostEnt != NULL)
		{
			sprintf(szHostaddress, "%d.%d.%d.%d", (pHostEnt->h_addr_list[0][0] & 0x00ff), (pHostEnt->h_addr_list[0][1] & 0x00ff), (pHostEnt->h_addr_list[0][2] & 0x00ff), (pHostEnt->h_addr_list[0][3] & 0x00ff));
		}
	}
	else {
		return;
	}
}