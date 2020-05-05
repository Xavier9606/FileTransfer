#include "headers/SocketsAPI.h"
#include "headers/FileInfo.h"




int SocketsAPI::initServer(int port) {

	isServer = 1;

	WSAStartup(MAKEWORD(2, 0), &WSAData);

	server = socket(AF_INET, SOCK_STREAM, 0);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr));	
	listen(server, 0);

	struct sockaddr_in sin,myAddr;
	int addrlen = sizeof(sin);
	if (getsockname(server, (struct sockaddr*) & sin, &addrlen) == 0 &&
		sin.sin_family == AF_INET &&
		addrlen == sizeof(sin))
	{
		int local_port = ntohs(sin.sin_port);
		wchar_t myIP[16];
		PWSTR myIPPW=myIP;

		memset(&myAddr, 0, sizeof(myAddr));
		InetNtopW(AF_INET, &myAddr.sin_addr, myIP, 16);
		std::cout << "\n Getsockname " << local_port << "\n";
		std::cout << "\n IP " << *InetNtopW(AF_INET, &myAddr.sin_addr, myIP, 16) << "\n";
	}
	else {
		std::cout << "\n error " << "\n";
	}
		

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

int SocketsAPI::connectToServ(const char* ip, u_short port) {

	if (isServer) {
		std::cout << "This instance of SocketsAPI is already a server!" << std::endl;
		return 0;
	}

	WSAStartup(MAKEWORD(2, 0), &WSAData);

	server = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_addr.s_addr = inet_addr(ip);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	connect(this->server, (SOCKADDR*)&addr, sizeof(addr));
	listen(this->server, 0);


	struct sockaddr_in sin, myAddr;
	int addrlen = sizeof(sin);
	if (getsockname(server, (struct sockaddr*) & sin, &addrlen) == 0 &&
		sin.sin_family == AF_INET &&
		addrlen == sizeof(sin))
	{
		int local_port = ntohs(sin.sin_port);
		wchar_t myIP[16];
		PWSTR myIPPW = myIP;

		memset(&myAddr, 0, sizeof(myAddr));
		InetNtopW(AF_INET, &myAddr.sin_addr, myIP, 16);
		std::cout << "\n Getsockname " << local_port << "\n";
		std::cout << "\n IP " << *myIP << "\n";
	}
	else {
		std::cout << "\n error " << "\n";
	}


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

char* SocketsAPI::servReceiveFile(const char* destPath, SocketsAPI* thisClient, const char* ip, u_short port) {

	if (!isServer) {
		std::cout << "This instance of SocketsAPI is not a server!" << std::endl;
		return 0;
	}
	while (true) {
		fd_set copy = master;

		int socketCount = select(NULL, &copy, nullptr, nullptr, nullptr);
		for (int i = 0; i < socketCount; ++i) {
			SOCKET sock = copy.fd_array[i];
			if (sock == server) {
				SOCKET client = accept(server, (SOCKADDR*) nullptr, nullptr);
				FD_SET(client, &master);
				std::cout << std::endl << "New active client! " << client << std::endl;
				//  std::cout << std::endl << thisClient->getRawSocket() << client << std::endl;


				char error_code;
				int error_code_size = sizeof(error_code);

			}
			else {

				// char* buffer = new char[bufferSize];
				 //ZeroMemory(buffer, bufferSize);
				this->clearBuffer();
				int err = recv(sock, buffer, bufferSize, 0); ;

				//tempBuff = this->receiveMsg(&err);
				if (err <= 0) {
					//  delete[] buffer;
					closesocket(sock);
					FD_CLR(sock, &master);
					std::cout << std::endl << "Socket " << sock << " connection destroyed! " << std::endl;
					continue;
				};
				// if (err == 0) continue;
				std::cout << "received" << std::endl;
				std::cout.flush();

				int filePositionOffset = 0;
				std::string fileSizeString = "";
				std::string fileNameString = "";
				int filesize = 0;
				int headLasts = 0;
				for (int i = 0; i < bufferSize; i++)
				{
					if (buffer[i] == '\n') {
						if (buffer[i + 1] == '\n') {
							filePositionOffset = i + 2;
						}

						if (!headLasts) {
							for (int j = 0; j < i; j++) {
								fileSizeString += buffer[j];
							}
							std::from_chars(fileSizeString.data(), fileSizeString.data() + fileSizeString.size(), filesize);
							std::cout << "FILESIZE IS: " << filesize << std::endl;
							headLasts = i + 1;
							continue;
						}

						for (int j = headLasts; j < filePositionOffset - 2; j++) {
							fileNameString += buffer[j];
						}
						break;
					}

				}
				// std::cout << std::endl << "FROM IS: " << filePositionStart << std::endl;
				FILE* dest;

				std::string finalFileName = destPath + fileNameString;
				if ((dest = fopen(finalFileName.c_str(), "wb")) == NULL) {
					std::cout << "CANT OPEN/CREATE DEST FILE!" << std::endl;;
					return 0;
				}

				fwrite(buffer + filePositionOffset, filesize, 1, dest);
				fflush(dest);
				fclose(dest);
				std::cout << std::endl << "File " << fileNameString << " successfully writen!" << std::endl << std::endl;
				//  delete[] buffer;
			}
		}
	}
}


char* SocketsAPI::servReceiveFileInChunks(MySafeQueue* queue)
{
	if (!isServer) {
		std::cout << "This instance of SocketsAPI is not a server!" << std::endl;
		return 0;
	}


	FileInfo recvdFileInfo;
	int isReady = 0;

	while (true) {
		fd_set copy = master;

		int socketCount = select(NULL, &copy, nullptr, nullptr, nullptr);
		for (int i = 0; i < socketCount; ++i) {
			SOCKET sock = copy.fd_array[i];
			if (sock == server) {
				SOCKET client = accept(server, (SOCKADDR*) nullptr, nullptr);
				FD_SET(client, &master);
				std::cout << std::endl << "New active client! " << client << std::endl;
				//  std::cout << std::endl << thisClient->getRawSocket() << client << std::endl;


				char error_code;
				int error_code_size = sizeof(error_code);

			}
			else {

				clearBuffer();
				int err = recv(sock, buffer, bufferSize, 0); ;

				//tempBuff = this->receiveMsg(&err);
				if (err <= 0) {
					//  delete[] buffer;
					closesocket(sock);
					FD_CLR(sock, &master);
					std::cout << std::endl << "Socket " << sock << " connection destroyed! " << std::endl;
					continue;
				};
				// if (err == 0) continue;
				char* newChunk = new char[1024 * 1024 + 300];
		
					std::cout << "received" << std::endl;
					memcpy(newChunk, buffer, 1024 * 1024 + 300);
					queue->push(newChunk);
				


			}
		}
	}
}

void SocketsAPI::setBufferSize(int size) {
	delete[] buffer;
	bufferSize = size;
	buffer = new char[bufferSize];
}

int SocketsAPI::getBufferSize() {
	return bufferSize;
}

SOCKET SocketsAPI::getRawSocket() {
	return this->server;
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