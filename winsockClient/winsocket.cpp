#include "winsocket.h"
#include <sstream>
/*void receving(SOCKET sock);
void sending(SOCKET sock);
message structReceive(SOCKET sock);//���� ����ü ����
void structSend(SOCKET sock, message msg);//���� ����ü �۽�
int recvn(SOCKET s, char* buf, int len, int flags);//����� ���� ������ �����Լ�
*/
// ����� ���� ������ ���� �Լ�
int winsocket::recvn(SOCKET s, char* buf, int len, int flags)
{

	int received; // recv() �Լ��� ���� ���� ������ ����
	char* ptr = buf;// ���� ���α׷� ������ ���� �ּ�. �����͸� ���� ������ ptr���� ���� ��Ų��.
	int left = len;// ���� ���� ���� ������ ũ���. �����͸� ���� ������ left�� ���� �Ѵ�.

	while (left > 0) // ���� ���� �ʴ� �����Ͱ� ������ ��� ������ ����. 
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		// recv �Լ��� ȣ���� ������ �߻��ϸ� ��ٷ� ���� �����Ѵ�.

		else if (received == 0)
			break;
		//�Լ��� ���� �����ϸ� �������� ���´�.

		left -= received;
		ptr += received;
		//�����͸� ���� ������ ���� , ���Ҹ� �ϸ鼭 ��Ȯ�� ����Ʈ ���� �����ϱ� ����.
	}

	return (len - left);//���� ����Ʈ ���� ����
}
winsocket::winsocket(const char* ip, int port) {

	if (ip == "" || port == 0) {
		cout << "invalid socket value(ip,port) input" << endl;
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		cout << "WSAStartup Error" << endl;
	sock = socket(PF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
		cout << "Socket Error" << endl;

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	//ServAddr.sin_addr.s_addr = inet_addr(ip);
	inet_pton(AF_INET, ip, &ServAddr.sin_addr.s_addr); //�������� ����ġ��.

	ServAddr.sin_port = htons(port); //�̰� ���߿� ��ɾ� Ȯ�� �ȵǸ� �˻��ϰ� ��ġ���.,

	cout << "connecting" << endl;
	if (connect(sock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
		cout << "connect Error" << endl;
	else
	{// ���ο� ������� �����Ѵ�.  �ѹ� ����Ǹ� �翬�� �� �ʿ伺�� ����.
		cout << "input nickname" << endl;
		cin >> nickname;
		cout << "nickname is " << nickname<<endl;
		//recvOpen = thread(&winsocket::receving,this,sock);
		receving(sock);
	}


	//recvOpen.join();


}


void winsocket::receving(SOCKET sock) {
	int len;
	message msg;
	thread sendOpen;

	while(1){

	sendOpen = thread(&winsocket::sending, this, sock);//���۱�� �� ���� ����.
	//�����غ��� ���⿡ ������ ���Ź޴� ������� �ϳ� ������ ��ð����ϴ� �ڵ��ٿ�Ǵ�...
	//���� ���ÿ� ������ ��ùߵ��ؾ��ϴµ� ���� ����ü�ϰ� ���� �����ϳ�.


	msg = structReceive(sock);// ���ӿ����� char��ȯ�� �ʼ�����.
	
	


	
		/*		
		msg.functiontype

		1. ��� �� ����Ʈ ���(�������� ������ ����Ʈ ��ȯ.  �� ����Ʈ �������� ��ȯ)
		2. ���� ����( �������� �������� Ȯ�� �޼��� ��ȯ)
		3. ��ȭ�� ����( �������� ���ӿϷ� �޼��� ��ȯ)
		//4. ��ȭ�� ����(�������� �����Ϸ� �޼��� ��ȯ) �ʿ����.
		5. ��� �޼��� ����.	

		*/

	switch (msg.functionType) {


	case 1 :
		cout << "all list " << endl;
		cout << msg.strBuffer << endl;//����� ó���Ǿ��ٸ� ��� ����Ʈ�� ����̴�.
		break;
	case 2:
		cout << "client delete complete from server " << endl;
		cout << msg.strBuffer << endl; // ����� ó���Ǿ��ٸ� ���� �Ϸ� ���ڰ� ����̴�.
		break;
	case 3:
		cout << "room selection complete " << endl;
		cout << msg.strBuffer << endl; // ����� ó���Ǿ��ٸ� ���� �Ϸ� ���ڰ� ����̴�.
		break;
	case 4:
		perror("room creation complete ");
		//cout << msg.strBuffer << endl; // ����� ó���Ǿ��ٸ� ���� �Ϸ� ���ڰ� ����̴�.
		break;
	case 5:
		cout << "communication goes well." << endl;
		cout << msg.strBuffer << endl; // ����� ó���Ǿ��ٸ� �г���+���� �� ����̴�.
		break;
	default:
		cout << "invalid response" << endl;
		break;

	}

	sendOpen.join();



	}


}
struct message winsocket::structReceive(SOCKET sock) {
	int retval;
	int len;

	retval = recvn(sock, (char*)&len, sizeof(int), 0); // ������ �ޱ�(���� ����)
	if (retval == SOCKET_ERROR) {
		cout << "recv()" << endl;
	}

	int GetSize;
	char suBuffer[1500];//����ũ��� ����ü�� �� ���� ���������� �����Ӱ�.
	message* msg;
	GetSize = recv(sock, suBuffer, len, 0);
	if (GetSize == SOCKET_ERROR) {
		exit(1);
	}

	suBuffer[GetSize] = '\0';
	msg = (message*)suBuffer;

	return *msg;


//��ó: https://windowshyun.tistory.com/5 [;]

}


void winsocket::sending(SOCKET sock) {
	
		message msg = message();//����ü �ʱ�ȭ.

		
		int talkRoomNum;
		//char nickname[32];
		char strBuffer[1000];
		char fileName[100];
		cout << "select the function num below\n\n" << endl;

		/*


		*/

		cout << "1. ��� �� ����Ʈ ���." << endl;
		cout << "2. ���� ����" << endl;
		cout << "3. ��ȭ�� �ѹ�����" << endl;//�����غ��� �ѹ������� ����� ���ϰ� �־���.
		cout << "4. �������� " << endl;
		cout << "5. ��� �޼��� ����." << endl;

		cin >> functionSelect;
		
		switch (functionSelect) {

		case 1://all room list
			msg.functionType = 1;
			structSend(sock, msg);
			break;

		case 2://disconnect
			msg.functionType = 2;
			structSend(sock, msg);
			return;
			break;
		case 3: // select room
			msg.functionType = 3;
			cout << "select room no " << endl;
			cin >> talkRoomNum;
			msg.Roomnum = talkRoomNum;
			structSend(sock, msg);
			break;

		case 4://file sender
			
			



			break;
			
		case 5: //normal message
			msg.functionType = 5;
			cout << "input message" << endl;
			cin >> strBuffer;
			strcpy_s(msg.strBuffer, strBuffer);
			strcpy_s(msg.nickname, nickname);
			cout << "msg.nickname " << nickname << endl;

			structSend(sock, msg);

			break;

		default:
			cout << "invalid function selection " << endl;
			break;




		}


		return;

	
}

void winsocket::fileReceive(SOCKET sock) {

}


void winsocket::structSend(SOCKET sock, message msg) {
	//int iferr = send(sock, (char*)&msg, sizeof(msg), 0);//ã�ƺôµ� ���� ������ �ϴ�.
	// https://windowshyun.tistory.com/5
	//if (iferr <= 0)
		//cout << "some error happened" << endl;//iferr�� ����Ʈ���� �������̹Ƿ�.

	int retval;
	// ������ ������( ����ü ũ�⸦ ���� ������. )
	int len = sizeof(msg);
	retval = send(sock, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		cout<<"send()"<<endl;
		exit(1);
	}
	// ������ ������( ����ü �����͸� ������. )
	retval = send(sock, (char*)&msg, sizeof(message), 0);
	if (retval == SOCKET_ERROR) {
		cout << "send()" << endl;
		exit(1);
	}


//��ó: https://windowshyun.tistory.com/5 [;]




	return;
}


