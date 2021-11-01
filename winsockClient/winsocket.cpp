#include "winsocket.h"
#include <sstream>
/*void receving(SOCKET sock);
void sending(SOCKET sock);
message structReceive(SOCKET sock);//실제 구조체 수신
void structSend(SOCKET sock, message msg);//실제 구조체 송신
int recvn(SOCKET s, char* buf, int len, int flags);//사용자 정의 데이터 수신함수
*/
// 사용자 정의 데이터 수신 함수
int winsocket::recvn(SOCKET s, char* buf, int len, int flags)
{

	int received; // recv() 함수의 리턴 값을 저장할 변수
	char* ptr = buf;// 응용 프로그램 버퍼의 시작 주소. 데이터를 읽을 때마다 ptr값을 증가 시킨다.
	int left = len;// 아직 읽지 않은 데이터 크기다. 데이터를 읽을 때마다 left을 감소 한다.

	while (left > 0) // 아직 읽지 않는 데이터가 있으면 계속 루프를 돈다. 
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		// recv 함수를 호출해 오류가 발생하면 곧바로 에러 리턴한다.

		else if (received == 0)
			break;
		//함수가 정상 종료하면 루프에서 나온다.

		left -= received;
		ptr += received;
		//데이터를 읽을 때마다 증가 , 감소를 하면서 정확한 바이트 수를 리턴하기 위함.
	}

	return (len - left);//읽은 바이트 수를 리턴
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
	inet_pton(AF_INET, ip, &ServAddr.sin_addr.s_addr); //신형으로 갈아치움.

	ServAddr.sin_port = htons(port); //이건 나중에 명령어 확인 안되면 검색하고 고치기로.,

	cout << "connecting" << endl;
	if (connect(sock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
		cout << "connect Error" << endl;
	else
	{// 새로운 스레드로 생성한다.  한번 연결되면 재연결 할 필요성은 없다.
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

	sendOpen = thread(&winsocket::sending, this, sock);//전송기는 또 따로 돈다.
	//생각해보니 여기에 파일을 수신받는 스레드로 하나 있으면 상시감시하다 자동다운되니...
	//파일 감시용 스레드 상시발동해야하는데 오는 구조체하고 어찌 구분하냐.


	msg = structReceive(sock);// 윈속에서는 char변환이 필수적임.
	
	


	
		/*		
		msg.functiontype

		1. 모든 방 리스트 출력(서버에서 접속자 리스트 반환.  방 리스트 서버에서 반환)
		2. 접속 종료( 서버에서 접속종료 확인 메세지 반환)
		3. 대화방 선택( 서버에서 접속완료 메세지 반환)
		//4. 대화방 생성(서버에서 생성완료 메세지 반환) 필요없음.
		5. 통상 메세지 전송.	

		*/

	switch (msg.functionType) {


	case 1 :
		cout << "all list " << endl;
		cout << msg.strBuffer << endl;//제대로 처리되었다면 모든 리스트가 뜰것이다.
		break;
	case 2:
		cout << "client delete complete from server " << endl;
		cout << msg.strBuffer << endl; // 제대로 처리되었다면 삭제 완료 문자가 뜰것이다.
		break;
	case 3:
		cout << "room selection complete " << endl;
		cout << msg.strBuffer << endl; // 제대로 처리되었다면 선택 완료 문자가 뜰것이다.
		break;
	case 4:
		perror("room creation complete ");
		//cout << msg.strBuffer << endl; // 제대로 처리되었다면 생성 완료 문자가 뜰것이다.
		break;
	case 5:
		cout << "communication goes well." << endl;
		cout << msg.strBuffer << endl; // 제대로 처리되었다면 닉네임+문자 가 뜰것이다.
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

	retval = recvn(sock, (char*)&len, sizeof(int), 0); // 데이터 받기(고정 길이)
	if (retval == SOCKET_ERROR) {
		cout << "recv()" << endl;
	}

	int GetSize;
	char suBuffer[1500];//버퍼크기는 구조체가 다 들어갈수 있을정도로 여유롭게.
	message* msg;
	GetSize = recv(sock, suBuffer, len, 0);
	if (GetSize == SOCKET_ERROR) {
		exit(1);
	}

	suBuffer[GetSize] = '\0';
	msg = (message*)suBuffer;

	return *msg;


//출처: https://windowshyun.tistory.com/5 [;]

}


void winsocket::sending(SOCKET sock) {
	
		message msg = message();//구조체 초기화.

		
		int talkRoomNum;
		//char nickname[32];
		char strBuffer[1000];
		char fileName[100];
		cout << "select the function num below\n\n" << endl;

		/*


		*/

		cout << "1. 모든 방 리스트 출력." << endl;
		cout << "2. 접속 종료" << endl;
		cout << "3. 대화방 넘버선택" << endl;//생각해보니 넘버생성이 기능을 겸하고 있었다.
		cout << "4. 파일전송 " << endl;
		cout << "5. 통상 메세지 전송." << endl;

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
	//int iferr = send(sock, (char*)&msg, sizeof(msg), 0);//찾아봤는데 문제 없을듯 하다.
	// https://windowshyun.tistory.com/5
	//if (iferr <= 0)
		//cout << "some error happened" << endl;//iferr는 바이트수로 찍힐것이므로.

	int retval;
	// 데이터 보내기( 구조체 크기를 먼저 보낸다. )
	int len = sizeof(msg);
	retval = send(sock, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		cout<<"send()"<<endl;
		exit(1);
	}
	// 데이터 보내기( 구조체 데이터를 보낸다. )
	retval = send(sock, (char*)&msg, sizeof(message), 0);
	if (retval == SOCKET_ERROR) {
		cout << "send()" << endl;
		exit(1);
	}


//출처: https://windowshyun.tistory.com/5 [;]




	return;
}


