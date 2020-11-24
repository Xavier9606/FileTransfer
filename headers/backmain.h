#ifndef BACKMAIN_H
#define BACKMAIN_H

#include <QObject>

#include <QFile>
#include <QDataStream>
#include <QString>
#include <QFileInfo>
#include <QDebug>
#include <QByteArray>

//Standart/Windows
#include <winsock2.h>
#include "stdio.h"
#include <iostream>
#include <charconv>
#include <thread>
#include <mutex>
#include <filesystem>
#include <queue>


//My API's & etc
#include "headers/SocketsAPI.h"
#include "headers/FileInfo.h"
#include "headers/MyFileAPI.h"
#include "headers/TransferHandler.h"
#include "headers/SafeQueue.h"

#define DEBUG

class BackMain : public QObject
{
    Q_OBJECT


    const int CHUNKSIZE = 1024 * 1024;
    SocketsAPI thisClientSock;
    SocketsAPI thisServerSock;
    int BUFFSIZE = CHUNKSIZE * 3;
    int isRecv = 0;
    std::string ip = "";
    int portOut = 0;
    int portSelf = 0;


    TransferHandler sender;
    TransferHandler receiver;

    MySafeQueue chunkQueue;

public:
    const char* SOURCEPATH = "C:\\Users\\Administrator\\Desktop\\FileTranserTest\\Source.jpg";
    std::string DESTPATH = "C:\\Users\\Administrator\\Desktop\\Received\\";

    BackMain();
    void init(int portOut);
    void receiveLoop();
    void writeLoop();
    void connect(int portOut,std::string ip);
    void send(std::string srcFilePath);

};

#endif // BACKMAIN_H
