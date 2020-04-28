#pragma once
#include "headers/SocketsAPI.h"
#include "headers/FileInfo.h"
#include "headers/MyFileAPI.h"
#include <queue>


class TransferHandler {

public:

	FileInfo recvdFileInfo;
	int isReady = 0;

	int sendFileInChunks(const char* path, const int CHUNKSIZE, SocketsAPI* destSocket);

	int recvFileInChunks(std::queue<char*>* queue);

};