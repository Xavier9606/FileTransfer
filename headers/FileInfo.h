#pragma once

#include "headers/MyFileAPI.h"
#include "headers/ChunkInfo.h"


/*	IMPLENTATION:
FileInfo sourceFileInfo;
sourceFileInfo.initForSend(Sourcepath, CHUNKSIZE);

for (int i = 0; i < sourceFileInfo.getChunksCount(); i++)													Send
{
sourceFileInfo.createChunk(i);
}

FileInfo recvdFileInfo;
recvdFileInfo.initForRecv(AnyReceivedChunkData, DestPath, TEMP_EXT, mapFilePath, MAP_EXT);
																											Receive
while (!recvdFileInfo.isComplete())
{
 recvdFileInfo.writeChunk(Received Chunk)
}
recvdFileInfo.resolveFile();
*/

class FileInfo {

private:
	const int  CHUNKINFO_INIT = 2;
	const int  READ_CHUNK = 1;
	const int  MAKE_CHUNK = 0;


	std::string fileFullPath = "";
	std::string tempFileFullPath = "";
	std::string fileName = "";
	std::string mapFilePath = "";
	int fullFileSize = 0;
	int chunkSize = 0;
	int chunksSaved = 0;

	int chunksCount = 0;
	int bytesLeft = 0;
	int bytesLeftFlag = 0;

	int bindedToMap = 0;
	int complete = 0;

	int state = 0;

	FILE* thisFilePointer = nullptr;
	FILE* thisMapFilePointer = nullptr;

	ChunkInfo* chunkInfo = nullptr;

	void initChunksCountInfo();

	FILE* createMapFile();

	int updateMapFile();

	int writeChunkDataToFile();

	char* readChunkData(int chunkMapPos);

	char* readChunkData(ChunkInfo* chunkInfo, int chunkMapPos, FILE* srcFile);

public:
	int isComplete() {
		return complete;
	}

	int getFileSize() {
		return fullFileSize;
	}

	int getBytesLeft() {
		return bytesLeft;
	}

	int initForRecv(char* anyRecvdChunk, std::string fileSavePath, std::string tempExt, std::string mapPath, std::string mapExt);

	int initForSend(std::string sourceFileFullPath, int chunkSize);

	char* createChunk(int mapPos);

	int writeChunk(char* recvdChunk);

	int resolveFile();

	int fileContainsChunk(char* chunk);

	int fileContainsChunk(ChunkInfo* chunkInfoToCompare, FILE* srcFile);

	int getChunksCount();

	~FileInfo();

};

