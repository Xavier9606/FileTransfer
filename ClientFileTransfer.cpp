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
const char* DESTPATH = "C:\\Users\\Administrator\\Desktop\\Received\\Result.jpg";


void receiveLoop() {

    while (true) {
        thisServerSock.servReceiveFile(DESTPATH);
    }
 
}


int main() {

    int portSelf=0;
    std::cout << "Enter your open port to create your server: ";
    std::cin >> portSelf;

    thisServerSock.initServer(portSelf);
    std::thread RecvTH(receiveLoop);
    RecvTH.detach();

    std::string ip = "";
    std::cout << "Enter IP of a buddy: ";
    std::cin >> ip;

    int portOut = 0;
    std::cout << "Enter your buddy's server port: ";
    std::cin >> portOut;


    //int other = 0;
    //if (port == 5555) other = 5556;
    //if (port == 5556) other = 5555;
 
  

    thisClientSock.connectToServ(ip.c_str, portOut);

    std::string srcFilePath = "";
    std::cout << "Enter your file location: ";
    std::cin >> srcFilePath;

    const char* FILENAME = getMyFileName(srcFilePath);


    sendFile(SOURCEPATH, thisClientSock);

    std::cout << std::endl << "DONE!" << std::endl;

    Sleep(10000000);

    return 0;
}
