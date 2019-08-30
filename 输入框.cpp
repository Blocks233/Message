#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<winsock2.h>
#include<Windows.h>
#include<cmath>
#include<string>
#include<iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
int id,tid;
char iD[10];
struct hostent* pHE;

BOOL  GetRealIpByDomainName(char* szHost, char szIp[50][100], int* nCount)
{
	WSADATA wsaData;
	HOSTENT* pHostEnt;
	int nAdapter = 0;
	struct sockaddr_in   sAddr;
	if (WSAStartup(0x0101, &wsaData))
	{
		printf(" gethostbyname error for host:\n");
		return FALSE;
	}

	pHostEnt = gethostbyname(szHost);
	if (pHostEnt)
	{
		while (pHostEnt->h_addr_list[nAdapter])
		{
			memcpy(&sAddr.sin_addr.s_addr, pHostEnt->h_addr_list[nAdapter], pHostEnt->h_length);

			sprintf_s(szIp[nAdapter], "%s", inet_ntoa(sAddr.sin_addr));

			nAdapter++;
		}

		*nCount = nAdapter;
	}
	else
	{
		DWORD  dwError = GetLastError();
		*nCount = 0;
	}
	WSACleanup();
	return TRUE;
}


int main() {
	printf("请输入你的ID号\n");
	scanf("%s", &iD);
	for (int i = strlen(iD) - 1; i >= 0; i--) {
		id += (iD[i] - '0') * pow(10, (strlen(iD) - 1) - i);
	}

	printf("请输入对方ID号\n");
	scanf("%d", &tid);

	// 返回的域名对应实际IP的个数
	int nIpCount = 0;
	// 返回的域名对应实际I列表
	char szIpList[50][100];
	// 域名
	char szDomain[256] = { 0 };
	char szIp[1024] = { 0 };
	strcpy_s(szDomain, "mccolor.tpddns.cn");
	GetRealIpByDomainName(szDomain, szIpList, &nIpCount);

	printf("公共服务器IP=%s\n", szIpList[0]);



	WSADATA WSAData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &WSAData) != 0) {
		system("pause");
		exit(0);
	}
	SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == clientSocket) {
		printf("Socket error!\n");
		system("pause");
		exit(0);
	}

	sockaddr_in dstAddr;
	dstAddr.sin_family = AF_INET;
	dstAddr.sin_port = htons(tid);
	dstAddr.sin_addr.S_un.S_addr = inet_addr(szIpList[0]);
	sockaddr_in dstAddr2;
	dstAddr2.sin_family = AF_INET;
	dstAddr2.sin_port = htons(id);
	dstAddr2.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");


	printf("请输入数据\n");
	char Data[2048];
	char sendData[2048];
	while (true) {
		gets_s(Data, 2048);
		memset(sendData, 0, 2048);
		strcat(sendData, iD);
		strcat(sendData, " : ");
		strcat(sendData, Data);
		sendto(clientSocket, sendData, strlen(sendData), 0, (sockaddr*)& dstAddr, sizeof(dstAddr));
		sendto(clientSocket, sendData, strlen(sendData), 0, (sockaddr*)& dstAddr2, sizeof(dstAddr2));
		printf("成功 数据=%s\n", Data);
	}

	closesocket(clientSocket);
	WSACleanup();
	return 0;
}