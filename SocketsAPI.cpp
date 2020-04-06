#include "SocketsAPI.h"



int SocketsAPI::initServer(int port) {

    isServer = 1;

    WSAStartup(MAKEWORD(2, 0), &WSAData);

    server = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    listen(server, 0);
    FD_ZERO(&master);

    return server;
}

int SocketsAPI::servAccept()
{
    if (!isServer) {
        std::cout << "This instance of SocketsAPI is not a server!" << std::endl;
        return;
    }

        SOCKET client = accept(server, (SOCKADDR*) nullptr, nullptr);
        FD_SET(client, &master);
        std::cout << "New active client!";
        std::string welcomeMsg = "Welcome to the chat";
        send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
  

    return client;
}


void SocketsAPI::servAcceptLoop()
{
    if (!isServer) {
        std::cout << "This instance of SocketsAPI is not a server!" << std::endl;
        return;
    }


   // FD_SET(server, &master);

    while (true) {
                SOCKET client = accept(server, (SOCKADDR*) nullptr, nullptr);
                FD_SET(client, &master);
                std::cout << "New active client!";
                std::string welcomeMsg = "Welcome to the chat";
                send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0); 
    }

    return;
}

int SocketsAPI::connectToServ(const char* ip , u_short port) {

    if (isServer) {
        std::cout << "This instance of SocketsAPI is already a server!" << std::endl;
        return 0;
    }

    WSAStartup(MAKEWORD(2, 0), &WSAData);

    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr(ip);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    connect(server, (SOCKADDR*)&addr, sizeof(addr));
    listen(server, 0);

    return server;
}


void SocketsAPI::sendMsg(char* msg, int size) {
    memcpy(buffer, msg, size);
    send(server, buffer, size, 0);
    clearBuffer();
}

char* SocketsAPI::receiveMsg(int* err) {
    clearBuffer();
    *err = recv(server, buffer, bufferSize, 0);
    return buffer;
}

char* SocketsAPI::receiveMsg() {
    if (recv(server, buffer, bufferSize, 0) <= 0) { exit(0); };
    return buffer;
}

void SocketsAPI::setBufferSize(int size) {
    delete[] buffer;
    bufferSize = size;
    buffer = new char[bufferSize];
}

void SocketsAPI::destroyConnection() {
    closesocket(server);
    WSACleanup();
}

void SocketsAPI::clearBuffer() {
    memset(buffer, 0, bufferSize);
}

SocketsAPI::~SocketsAPI() {
    closesocket(server);
    WSACleanup();
}