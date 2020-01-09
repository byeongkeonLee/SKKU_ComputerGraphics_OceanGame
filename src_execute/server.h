
#include <WinSock2.h>
#include <iostream>
#include <thread>
#pragma comment (lib , "ws2_32.lib")

bool get_ip(char* str) {
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);

	PHOSTENT hostinfo;
	char hostname[50] = { 0 };
	char ipaddr[50] = { 0 };
	int nError= gethostname(hostname, sizeof(hostname));
	if (nError == 0) {
		hostinfo = gethostbyname(hostname);
		strcpy(str, inet_ntoa(*(struct in_addr*)hostinfo->h_addr_list[0]));
	}
	return false;
}