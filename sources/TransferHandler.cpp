#include "headers/TransferHandler.h"


int TransferHandler::sendFileInChunks(const char* path, const int CHUNKSIZE, SocketsAPI* destSocket) {

	FileInfo sourceFileInfo;
	sourceFileInfo.initForSend(path, CHUNKSIZE);
	char* chunk = nullptr;
	for (int i = 0; i < sourceFileInfo.getChunksCount(); i++) {
		std::cout << i << '\n';

		if (sourceFileInfo.getChunksCount() - 1 == i) {
			chunk = sourceFileInfo.createChunk(i);
			std::cout << "Send " << send(destSocket->getRawSocket(), chunk, sourceFileInfo.getBytesLeft() + 300, 0) << '\n';
			
		}
		else {
			chunk = sourceFileInfo.createChunk(i);
			std::cout << "Send " << send(destSocket->getRawSocket(), chunk, CHUNKSIZE + 300, 0) << '\n';
			delete[] chunk;
		}
	}

	sourceFileInfo.sendEnd();

	return 0;
}

int TransferHandler::writeFileFromChunks(MySafeQueue* queue, std::string destPath)
{

	const std::string TEMP_EXT = ".inprogress"; //temp file extension until file is not downloaded

	const std::string mapFilePath("C:\\Users\\Administrator\\Desktop\\Received\\"); //temp map file path
	const std::string MAP_EXT = ".chunkmap"; //map file extention

	while (true)
	{
		if (!queue->empty()) {



			if (nullptr == recvdFileInfo) {
				recvdFileInfo = new FileInfo;
				recvdFileInfo->initForRecv(queue->front(), destPath, TEMP_EXT, mapFilePath, MAP_EXT);
				isReady = 1;
			}

			if (0 != recvdFileInfo->writeChunk(queue->front())) {
				std::cout << "Error while writing chunk!\n";
			}
			else {
				std::cout << "writen!\n" << std::endl;
				delete[] queue->front();
				queue->pop();
			}

			if (recvdFileInfo->isComplete()) {
				recvdFileInfo->solveFile();
				delete recvdFileInfo;
				recvdFileInfo = nullptr;
				return 0;
			}
		}
		Sleep(1); //for not loading CPU while chunks are downloaded but writing didn't started yet
	}
	return 0;
}
