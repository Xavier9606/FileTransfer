#include "headers/backmain.h"

BackMain::BackMain()
{

}


void BackMain::receiveLoop()
{

        thisServerSock.servReceiveFileInChunks(&chunkQueue);

}

void BackMain::writeLoop()
{


        while (true) {
            receiver.writeFileFromChunks(&chunkQueue, DESTPATH);
            Sleep(10);
        }

}

void BackMain::connect(int portOut, std::string ip)
{
#ifdef DEBUG
    ip = "192.168.88.254";
#endif // !debug


    if (portOut == 1) portOut = 5555;
    if (portOut == 2) portOut = 5556;

    thisClientSock.connectToServ(ip.c_str(), portOut);
    thisClientSock.setBufferSize(BUFFSIZE);
      std::cout<<"Connected on "<<portOut<<" port, ip:"<<ip<<std::endl;
}

void BackMain::send(std::string srcFilePath)
{

    if ("rc" == srcFilePath) {
        std::cout << "\nReconection\n";
        thisClientSock.connectToServ(ip.c_str(), portOut);
        thisClientSock.setBufferSize(BUFFSIZE);
        return;
    }
    if (srcFilePath.size()<3) {
        return;
    }

    sender.sendFileInChunks(srcFilePath.c_str(), CHUNKSIZE, &thisClientSock);

    std::cout << std::endl << "DONE!" << std::endl;
}

void BackMain::init(int portSelf)
{


        //int portSelf = 0;
        //std::cout << "Enter your open port to create your server: ";
       // std::cin >> portSelf;

        if (portSelf == 1) portSelf = 5555;
        if (portSelf == 2) portSelf = 5556;

        thisServerSock.initServer(portSelf);
        thisServerSock.setBufferSize(BUFFSIZE);
        std::thread RecvTH(&BackMain::receiveLoop, this);
        RecvTH.detach();
        std::thread WriteTH(&BackMain::writeLoop, this);
        WriteTH.detach();

        std::cout<<"Inited on "<<portSelf<<std::endl;

    #ifndef DEBUG


        std::cout << "Enter IP of a buddy: ";
        std::cin >> ip;


        std::cout << "Enter your buddy's server port: ";
        std::cin >> portOut;

    #endif // !debug

    #ifdef DEBUG

        if (portSelf == 5555) portOut = 5556;
        if (portSelf == 5556) portOut = 5555;
    #endif

    #ifndef DEBUG
        std::cout << "Where to save?: " << std::endl;
        std::cin >> DESTPATH;



        if ('\\' != DESTPATH.back() || '/' != DESTPATH.back()) {
            DESTPATH += '\\';
        }


    #endif // !debug




        return;

}



