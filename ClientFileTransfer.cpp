// ClientFileTransfer.cpp : Defines the entry point for the application.
//

//              The "Trademark name" Roadmap
/*
                         Pre-Alpha
                            MVP
DONE Direct transfer thru sockets protoype
     Server/client 2in1 simple API
     Filename transfer (+file info);
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


SocketsAPI thisClientSock;
SocketsAPI thisServerSock;
int BUFFSIZE = 5000000;
int isRecv = 0;

const char* SOURCEPATH = "C:\\Users\\Administrator\\Desktop\\FileTranserTest\\Source.jpg";
const char* DESTPATH = "C:\\Users\\Administrator\\Desktop\\Received\\";


void receiveLoop() {

        thisServerSock.servReceiveFile(DESTPATH);
   
 
}


int main() {

    int portSelf=0;
    std::cout << "Enter your open port to create your server: ";
    std::cin >> portSelf;

    thisServerSock.initServer(portSelf);
    thisServerSock.setBufferSize(BUFFSIZE);
    std::thread RecvTH(receiveLoop);
    RecvTH.detach();

    std::string ip = "";
    //std::cout << "Enter IP of a buddy: ";
    //std::cin >> ip;

    int portOut = 0;
    //std::cout << "Enter your buddy's server port: ";
    //std::cin >> portOut;


    int other = 0;
    if (portSelf == 5555) portOut = 5556;
    if (portSelf == 5556) portOut = 5555;
 
    char error_code;
    int error_code_size = sizeof(error_code);
    //"192.168.88.254"
    thisClientSock.connectToServ("192.168.88.254", portOut);
    thisClientSock.setBufferSize(BUFFSIZE);


    while (true) {
        std::cout << getsockopt(thisClientSock.getRawSocket(), SOL_SOCKET, SO_ERROR, &error_code, &error_code_size) << std::endl;
        if (-1==getsockopt(thisClientSock.getRawSocket(), SOL_SOCKET, SO_ERROR, &error_code, &error_code_size)) {
            thisClientSock.connectToServ("192.168.88.254", portOut);
            thisClientSock.setBufferSize(BUFFSIZE);
        }
        std::string srcFilePath = "";
        //std::filesystem::path srcFilePath = "";
        std::cout << "Enter your file location: ";
        std::cin >> srcFilePath;

        //const char* FILENAME = getMyFileName(srcFilePath);


        sendFile(srcFilePath.c_str(), thisClientSock);

        std::cout << std::endl << "DONE!" << std::endl;
    }

    return 0;
}
