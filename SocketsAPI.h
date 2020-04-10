#pragma once
#include <winsock2.h>
#include <iostream>
#include <charconv>
//#include "MyFileAPI.h"

class SocketsAPI{
private:
    WSADATA WSAData;
    SOCKET server;
    SOCKET client;
    SOCKADDR_IN addr;
    SOCKADDR_IN serverAddr;
    int bufferSize = 5000000;
    char* buffer = new char[bufferSize];
    int isServer = 0;
    fd_set master;

    //char buffer[1024] ;


public:
    int connActive = 0;

    int initServer(int port);

    int servAccept();

    void servAcceptLoop();

    int connectToServ(const char* ip = "127.0.0.1", u_short port = 5555);

    void sendMsg(char* msg, int size);

    char* receiveMsg(int* err);

    char* receiveMsg();

    char* servReceiveFile(const char* destPath);

    void setBufferSize(int size);

    void clearBuffer();

    void destroyConnection();

    ~SocketsAPI();
};



