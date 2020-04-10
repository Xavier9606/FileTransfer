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
    FD_SET(server, &master);

    return server;
}

int SocketsAPI::servAccept()
{
    if (!isServer) {
        std::cout << "This instance of SocketsAPI is not a server!" << std::endl;
        return 0;
    }
    
        SOCKET client = accept(server, (SOCKADDR*) nullptr, nullptr);
        connActive = 1;
        FD_SET(client, &master);
        std::cout << "New active client!";
       // std::string welcomeMsg = "Welcome to the chat";
       // send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
  

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
                client = accept(server, (SOCKADDR*) nullptr, nullptr);
                FD_SET(client, &master);
                std::cout << "New active client!";
               // std::string welcomeMsg = "Welcome to the chat";
               // send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0); 
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
    
    if (isServer) { *err = recv(client, buffer, bufferSize, 0); }
    else
    {
        *err = recv(server, buffer, bufferSize, 0);
    }
    return buffer;
}

char* SocketsAPI::receiveMsg() {
    
    if (recv(server, buffer, bufferSize, 0) <= 0) { exit(0); };
    return buffer;
}

char* SocketsAPI::servReceiveFile(const char* destPath) {
  
    if (!isServer) {
        std::cout << "This instance of SocketsAPI is not a server!" << std::endl;
        return 0;
    }

    fd_set copy = master;

    int socketCount = select(NULL, &copy, nullptr, nullptr, nullptr);
    auto tempBuff = new char[bufferSize];
    for (int i = 0; i < socketCount; ++i) {
        SOCKET sock = copy.fd_array[i];
        if (sock == server) {
            SOCKET client = this->client = accept(server, (SOCKADDR*) nullptr, nullptr);
            FD_SET(client, &master);
            std::cout << "New active client!";
        }
        else {

            char* tempBuff;
            int err = 1;
            tempBuff = this->receiveMsg(&err);
            if (err <= 0) { 
                //delete[] tempBuff;
                return 0; 
            };
           // if (err == 0) continue;
            std::cout << "received";

            int filePositionStart = 0;
            std::string fileSizeString = "";
            int filesize = 0;
            for (int i = 0; i < bufferSize; i++)
            {
                if (tempBuff[i] == '\n') {
                    filePositionStart = i+1;
                    for (int j = 0; j < i; j++) {
                        fileSizeString += tempBuff[j];
                    }
                    std::from_chars(fileSizeString.data(), fileSizeString.data() + fileSizeString.size(), filesize);
                    std::cout << std::endl << "FILESIZE IS: " << filesize << std::endl;
                    break;
                }

            }
           std::cout << std::endl << "FROM IS: " << filePositionStart << std::endl;
           FILE* dest;

           if ((dest = fopen(destPath, "wb")) == NULL) {
               std::cout << "CANT OPEN/CREATE DEST FILE!";
               return 0;
           }

           fwrite(tempBuff + filePositionStart, filesize, 1, dest);
           fflush(dest);
           fclose(dest);
           //delete[] tempBuff;
           std::cout << std::endl << "FILE WRITTEN!!!" << std::endl;
        }
    }
   // delete[] tempBuff;
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