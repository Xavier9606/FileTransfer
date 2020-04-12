// ClientFileTransfer.cpp : Defines the entry point for the application.
//

//              The "Trademark name" Roadmap
/*
                         Pre-Alpha
                            MVP
DONE Direct transfer thru sockets protoype
     Server/client 2in1 simple API
DONE Filename transfer (+file info);
     MD5Hash
     Chunking files (probably std::map int, char[1024])
     Login/connectivity server
     Resuming
                           Alpha
                          Refactor
     Qt refactor (All Qt from now on)

                          Advanced
     Simultaneous receiving and sending with resuming
     Multiple files transfer (in and out) with resuming
     Multiple destination transfer (in and out) with resuming

                            Beta
                       Pre-Production
     Qt basic visual interface (+naming)
     Desktop pop-up window (keyboard shortcut)
     Context menu integration
                         
                           Release
                          Producion
     Interface design rework (+icon)

                    Advanced production
     Crossplatform product API
     Web version
     Mobile version (Android/iOS?)
     Various Plug-in version realisation

*/
#include "ClientFileTransfer.h"

#define DEBUG

SocketsAPI thisClientSock;
SocketsAPI thisServerSock;
int BUFFSIZE = 50000000;
int isRecv = 0;
std::string ip = "";
int portOut = 0;

const char* SOURCEPATH = "C:\\Users\\Administrator\\Desktop\\FileTranserTest\\Source.jpg";
std::string DESTPATH = "C:\\Users\\Administrator\\Desktop\\Received\\";


void receiveLoop() {

        thisServerSock.servReceiveFile(DESTPATH.c_str(),&thisClientSock, "192.168.88.254", portOut);
   
 
}
bool is_connected(int sock)
{
    char buf;
    int err = recv(sock, &buf, 1, MSG_PEEK);
    return err == -1 ? false : true;
}

int main() {

    int portSelf=0;
    std::cout << "Enter your open port to create your server: ";
    std::cin >> portSelf;

    if (portSelf == 1) portSelf = 5555;
    if (portSelf == 2) portSelf = 5556;

    thisServerSock.initServer(portSelf);
    thisServerSock.setBufferSize(BUFFSIZE);
    std::thread RecvTH(receiveLoop);
    RecvTH.detach();

   
#ifndef DEBUG


    std::cout << "Enter IP of a buddy: ";
    std::cin >> ip;

 
    std::cout << "Enter your buddy's server port: ";
    std::cin >> portOut;

#endif // !debug 

    int other = 0;

    if (portSelf == 5555) portOut = 5556;
    if (portSelf == 5556) portOut = 5555;


#ifndef DEBUG
    std::cout << "Where to save?: " << std::endl;
    std::string destPathString = "";
    std::cin >> destPathString;



    if ('\\' != destPathString.back() || '/' != destPathString.back()) {
        destPathString += '\\';
    }

    DESTPATH = destPathString.c_str();

#endif // !debug 

    std::cout << std::endl << "Enter anything to connect ";
    std::string test = "";
    std::cin >> test;

#ifdef DEBUG
    ip = "192.168.88.254";
#endif // !debug 

    thisClientSock.connectToServ(ip.c_str(), portOut);
    thisClientSock.setBufferSize(BUFFSIZE);





    while (true) {

        std::string srcFilePath = "";
        //std::filesystem::path srcFilePath = "";
        std::cout << "Enter your file location: ";
        std::cin >> srcFilePath;

        sendFile(srcFilePath.c_str(), &thisClientSock);

        std::cout << std::endl << "DONE!" << std::endl;
    }

    return 0;
}
