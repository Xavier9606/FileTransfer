// Main.cpp : Defines the entry point for the application.
//

//              The "Trademark name" Roadmap
/*
						 Pre-Alpha
							MVP
DONE Direct transfer thru sockets protoype
	 Server/client 2in1 simple API
DONE Filename transfer (+file info);
	 MD5Hash
DONE Chunking files (probably std::map int, char[1024])
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
#include "headers/Main.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "headers/backend.h"

#define DEBUG

const int CHUNKSIZE = 1024 * 1024;
SocketsAPI thisClientSock;
SocketsAPI thisServerSock;
int BUFFSIZE = CHUNKSIZE * 3;
int isRecv = 0;
std::string ip = "";
int portOut = 0;

const char* SOURCEPATH = "C:\\Users\\Administrator\\Desktop\\FileTranserTest\\Source.jpg";
std::string DESTPATH = "C:\\Users\\Administrator\\Desktop\\Received\\";


TransferHandler sender;
TransferHandler receiver;

MySafeQueue chunkQueue;



void receiveLoop() {

	thisServerSock.servReceiveFileInChunks(&chunkQueue);


}

void writeLoop() {
	while (true) {
		receiver.writeFileFromChunks(&chunkQueue, DESTPATH);
		Sleep(10);
	}
}

//int main(int argc, char* argv[]) {
//
//	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//
//	QGuiApplication app(argc, argv);
//
//    qmlRegisterType<BackEnd>("io.qt.examples.backend", 1, 0, "BackEnd");
//
//	QQmlApplicationEngine engine;
//	const QUrl url(QStringLiteral("qrc:/main.qml"));
//
//	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//		&app, [url](QObject* obj, const QUrl& objUrl) {
//			if (!obj && url == objUrl)
//				QCoreApplication::exit(-1);
//		}, Qt::QueuedConnection);
//	engine.load(url);
//
//	return app.exec();;
//}

int main() {

	int portSelf = 0;
	std::cout << "Enter your open port to create your server: ";
	std::cin >> portSelf;

	if (portSelf == 1) portSelf = 5555;
	if (portSelf == 2) portSelf = 5556;

	thisServerSock.initServer(portSelf);
	thisServerSock.setBufferSize(BUFFSIZE);
	std::thread RecvTH(receiveLoop);
	RecvTH.detach();
	std::thread WriteTH(writeLoop);
	WriteTH.detach();


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

		if ("rc" == srcFilePath) {
			std::cout << "\nReconection\n";
			thisClientSock.connectToServ(ip.c_str(), portOut);
			thisClientSock.setBufferSize(BUFFSIZE);
			continue;
		}	
		if (srcFilePath.size()<3) {
			continue;
		}

		sender.sendFileInChunks(srcFilePath.c_str(), CHUNKSIZE, &thisClientSock);

		std::cout << std::endl << "DONE!" << std::endl;
	}


	return 0;
}

//const int CHUNKSIZE = 1024 * 3; //bytes
//
//const std::string str1("C:\\Users\\Administrator\\Desktop\\FileTranserTest\\Source.rar"); //Source file
//
//const std::string str2("C:\\Users\\Administrator\\Desktop\\Received\\ChunkedRecv "); //Save path
//const std::string TEMP_EXT = ".inprogress"; //temp file extension until file is not downloaded
//
//
//const std::string mapFilePath("C:\\Users\\Administrator\\Desktop\\Received\\"); //temp map file path
//const std::string MAP_EXT = ".chunkmap"; //map file extention


/*
192.168.88.254
C:\\Users\\Administrator\\Desktop\\FileTranserTest\\Source.jpg
C:\\Users\\Administrator\\Desktop\\Received\\
*/