#pragma once
#include <winsock2.h>
#include <iostream>
#include <charconv>
#include <queue>
#include <string>
#include "headers/SafeQueue.h"
#include <ws2tcpip.h>

class SocketsAPI {
private:
	WSADATA WSAData;
	SOCKET server;
	SOCKET client;
	SOCKADDR_IN addr;
	SOCKADDR_IN serverAddr;
	int bufferSize = 1024*1024*3;
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

	char* servReceiveFile(const char* destPath, SocketsAPI* thisClient, const char* ip = "127.0.0.1", u_short port = 5555);

	char* servReceiveFileInChunks(MySafeQueue* queue);

	void setBufferSize(int size);

	int getBufferSize();

	SOCKET getRawSocket();

	void clearBuffer();

	void destroyConnection();

	~SocketsAPI();
};



