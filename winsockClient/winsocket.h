#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//#include <windows.h>
#include <winsock2.h>
//#include <ws2tcpip.h>

#include <string>
#include <cstring>
#include <thread>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include <WS2tcpip.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define PACKET_SIZE 1024

using namespace std;

class winsocket
{
public:
		WSADATA wsadata;
		SOCKET sock;
		SOCKADDR_IN ServAddr;
		//char message[100];
		
		winsocket(const char  *ip, int port);
		~winsocket();
		thread recvOpen,fileReceive_;
		FILE* fp =NULL;
		char nickname[32];
		

private:
	int functionSelect;
	int sendBytes;
	long file_size;
	char buf[PACKET_SIZE];

	void receving(SOCKET sock);
	void sending(SOCKET sock);
	struct message structReceive(SOCKET sock);//���� ����ü ����
	void structSend(SOCKET sock, message msg);//���� ����ü �۽�
	int recvn(SOCKET s, char* buf, int len, int flags);//����� ���� ������ �����Լ�
	void fileReceive(SOCKET sock);
};

struct message {

	int functionType;
	
	/*
	
	1. ��� �� ����Ʈ ���(�������� ������ ����Ʈ ��ȯ.  �� ����Ʈ �������� ��ȯ)
	2. ���� ����( �������� �������� Ȯ�� �޼��� ��ȯ)
	3. ��ȭ�� ����( �������� ���ӿϷ� �޼��� ��ȯ)
	4. ��ȭ�� ����(�������� �����Ϸ� �޼��� ��ȯ)
	5. ��� �޼��� ����.	
	
	*/
	char nickname[32]; //�ۼ��� �г��� ������.
	int Roomnum;
	char strBuffer[1000];

	message() {
		functionType = 0;
		memset(nickname, 0, 32);
		memset(strBuffer, 0, 1000);
		Roomnum = 0;
	}

};


