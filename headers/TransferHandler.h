#pragma once
#include "headers/SocketsAPI.h"
#include "headers/FileInfo.h"
#include "headers/MyFileAPI.h"
#include <queue>
#include "headers/SafeQueue.h"

class TransferHandler {

public:

	FileInfo* recvdFileInfo = nullptr;
	int isReady = 0;

	int sendFileInChunks(const char* path, const int CHUNKSIZE, SocketsAPI* destSocket);

	int writeFileFromChunks(MySafeQueue* queue, std::string destPath);

};