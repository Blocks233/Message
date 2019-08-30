#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<winsock2.h>
#include<Windows.h>
#include<openssl\md5.h>
#include<string>
#include<cstring>
#include<cmath>
#pragma comment(lib,"ws2_32.lib")

char key[40], iD[5],buf[40] = { '/0' };
int id;



void MD5(const char* src)
{

	MD5_CTX ctx;
	char* data = (char*)src;
	unsigned char md[16];
	char tmp[3] = { '/0' };
	int i;

	MD5_Init(&ctx);
	MD5_Update(&ctx, data, strlen(data));
	MD5_Final(md, &ctx);

	for (i = 0; i < 16; i++) {
		sprintf(tmp, "%02X", md[i]);
		strcat(buf, tmp);
	}
}

void FileInit(int p) {
	FILE* fp;
	fp = fopen("frpc.ini", "w");
	fprintf(fp,"[common]\n");
	fprintf(fp, "server_addr = vps.mccolor.cc\n");
	fprintf(fp, "server_port = 7000\n");
	fprintf(fp, "token = token_token\n");
	fprintf(fp, "\n");
	fprintf(fp, "[id_%d]\n",p);
	fprintf(fp, "type = udp\n");
	fprintf(fp, "local_ip = 127.0.0.1\n");
	fprintf(fp, "local_port = %d\n",p);
	fprintf(fp, "use_encryption = false\n");
	fprintf(fp, "use_compression = false\n");
	fprintf(fp, "remote_port = %d\n",p);
	fclose(fp);
}

int main() {
	printf("请输入你的ID号\n");
	scanf("%s", &iD);
	for (int i = strlen(iD) - 1; i >= 0; i--) {
		id += (iD[i] - '0') * pow(10, (strlen(iD) - 1)-i);
	}
	
	printf("ID=%d\n请输入你的密钥\n",id);
	scanf("%s", &key);

	MD5(iD);
	if (!strstr(buf,key)) {
		printf("ID号或密钥不正确\n");
		system("pause");
		exit(0);
	}


	FileInit(id);
	system("start frp.bat");

	WSAData wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa)!=0) exit(0)
;


	WSADATA WSAData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &WSAData) != 0) exit(0);

	SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == serSocket) {
		printf("socket error\n");
		system("pause");
		exit(0);
	}


	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(id);
	serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (bind(serSocket, (sockaddr*)& serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
		printf("bind error\n");
		system("pause");
		exit(0);
	}

	sockaddr_in clientAddr;
	int iAddrlen = sizeof(clientAddr);
	char buff[2048];


	printf("开始接收数据\n");

	while (true) {
		memset(buff, 0, 2048);

		int len = recvfrom(serSocket, buff, 2048, 0, (sockaddr*)& clientAddr, &iAddrlen);
		if (len > 0) {
			printf("ID=%s\n", buff);
		}
	}

	closesocket(serSocket);
	WSACleanup();

	return 0;
}