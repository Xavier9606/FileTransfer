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


int main() {
    const char* SOURCEPATH = "C:\\Users\\Administrator\\Desktop\\FileTranserTest\\Source.jpg";
    const char* DESTPATH = "C:\\Users\\Administrator\\Desktop\\FileTranserTest\\Result.jpg";


    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN serverAddr;

    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);

    bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

    listen(server, 0);

    ClientAPI client;
    client.initConnection();


    int sourcesize = getfilesize(SOURCEPATH);
    std::cout << std::endl << "sourcesize is " << sourcesize<< std::endl;
    client.setBufferSize(sourcesize);
    char* buffer = new char[sourcesize];
    if (!0 == readFileData(SOURCEPATH, sourcesize, buffer)) {
        return -1;
    };
    //writeFileData(DESTPATH, sourcesize, buffer);

    std::string str{ "xxxxxxxxxxxx" };
    std::to_chars(str.data(), str.data() + str.size(), sourcesize);

    std::string msg = std::to_string(sourcesize) + "\n";
    auto msgChars = new char[5000000];
    client.setBufferSize(5000000);
    memcpy(msgChars, msg.c_str(), sizeof(msg));
    memcpy(msgChars + msg.length(), buffer, sourcesize);
    client.sendMsg(msgChars, sourcesize);

    //writeFileData(DESTPATH, sourcesize, msgChars,6);

    std::cout << std::endl << "sourcesize = " << sourcesize << std::endl;
    auto charTest = new char[100];
    memcpy(charTest, static_cast<void*>(&sourcesize), sizeof(int));
    int newInt = 0;
    memmove(static_cast<void*>(&newInt), charTest, sizeof(int));
    newInt++;
    std::cout << std::endl << "Transfered int = " << newInt<<std::endl;
    std::cout << std::endl << "sizeof transfered int " << sizeof(newInt) <<std::endl;
    std::cout << std::endl << "sizeof static_cast<void*>(&newInt) " << sizeof(static_cast<void*>(&newInt)) <<std::endl;
    std::cout << std::endl << "typeid newInt " << typeid(static_cast<void*>(&newInt)).name() <<std::endl;
    std::cout << std::endl << "sizeof newInt " << sizeof(newInt) <<std::endl;

    std::cout << std::endl << "DONE!" << std::endl;


    return 0;
}
