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

char key[40], iD[5], buf[40] = { '/0' };
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

int main() {
	scanf("%s", key);
	MD5(key);
	printf("%s\n", buf);
	system("pause");
}