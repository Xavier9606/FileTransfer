#include "headers/TransferHandler.h"


int TransferHandler::sendFileInChunks(const char* path, const int CHUNKSIZE, SocketsAPI* destSocket) {

	const std::string str1("C:\\Users\\Administrator\\Desktop\\FileTranserTest\\Source.rar"); //Source file

	FileInfo sourceFileInfo;
	sourceFileInfo.initForSend(path, CHUNKSIZE);

	for (int i = 0; i < sourceFileInfo.getChunksCount(); i++) {
		std::cout << i << '\n';
		
		if (sourceFileInfo.getChunksCount()-1 == i){
			std::cout << "Send " << send(destSocket->getRawSocket(), sourceFileInfo.createChunk(i), sourceFileInfo.getBytesLeft() + 300, 0) << '\n';
		}
		else {
			std::cout << "Send " << send(destSocket->getRawSocket(), sourceFileInfo.createChunk(i), CHUNKSIZE + 300, 0) << '\n';
		}


		Sleep(100);
	}

	return 0;
}

int TransferHandler::recvFileInChunks(std::queue<char*>* queue)
{

	const std::string str2("C:\\Users\\Administrator\\Desktop\\Received\\ChunkedRecv "); //Save path
	const std::string TEMP_EXT = ".inprogress"; //temp file extension until file is not downloaded


	const std::string mapFilePath("C:\\Users\\Administrator\\Desktop\\Received\\"); //temp map file path
	const std::string MAP_EXT = ".chunkmap"; //map file extention

	while (true)
	{
		if (!queue->empty()) {


		
				if (!isReady) {
					recvdFileInfo.initForRecv(queue->front(), str2, TEMP_EXT, mapFilePath, MAP_EXT);
					isReady = 1;

					//if (0 != recvdFileInfo.writeChunk(queue->front())) {
					//	std::cout << "Error while writing chunk!\n";
					//}
					//else {
					//	std::cout << "writen!\n" << std::endl;
					//	//delete[] queue->front;
					//	queue->pop();
					//	continue;
					//}
				}
				//std::cout << "recvdFileInfo.getChunksCount() " << recvdFileInfo.getChunksCount() << '\n';
				//std::cout << "queue->size() " << queue->size() << '\n';
				//if (recvdFileInfo.getChunksCount() == queue->size()) {
					//std::cout << "received" << std::endl;
					if (0 != recvdFileInfo.writeChunk(queue->front())) {
						std::cout << "Error while writing chunk!\n";
					}
					else {
						std::cout << "writen!\n" << std::endl;
						queue->pop();
					}
				
			//}
			if (recvdFileInfo.isComplete()) {
				recvdFileInfo.resolveFile();
				int isReady = 0;
				return 0;
			}
		}
	}
	return 0;
}
