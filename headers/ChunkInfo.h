#pragma once
#include <iostream>

class ChunkInfo {

private:
	const int  CHUNKINFO_INIT = 2;
	const int  READ_CHUNK = 1;
	const int MAKE_CHUNK = 0;

int sendReady = 0;
public:

	int chunkSize = 0;				//
	std::string fileName = "";		//		mandatory when writing
	int sourceFileSize = 0;			//
	int chunkMapPos = 0;

	int chunkDataOffset = 0;
	int bytesLeft = 0;
	int chunksCount = 0;
	char* chunkRawFileData = nullptr;

	ChunkInfo(std::string fileName, int sourceFileSize, int chunkSize);

	ChunkInfo(char* chunkMsg);

	void initChunksCountInfo();

	int isSendReady();

	char* parseChunk(char* chunkMsg, int flag);

	~ChunkInfo();
};