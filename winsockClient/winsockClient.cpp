// winsockClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#define portnum 7889
#include "winsocket.h" //이거 함부러 썻다?winsocket.h에서도 portnum이 이미 있던걸로 인식되버려서 고생했다.
int main()
{   // I am client.
    winsocket* socket = new winsocket("127.0.0.1" , portnum);
    std::cout << "Hello World!\n";
    return 0;
}
