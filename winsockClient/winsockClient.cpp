// winsockClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#define portnum 7889
#include "winsocket.h" //�̰� �Ժη� ����?winsocket.h������ portnum�� �̹� �ִ��ɷ� �νĵǹ����� ����ߴ�.
int main()
{   // I am client.
    winsocket* socket = new winsocket("127.0.0.1" , portnum);
    std::cout << "Hello World!\n";
    return 0;
}
