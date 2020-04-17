#pragma once
#include "ChunkInfo.h"

#define CHUNKINFO_INIT 2
#define READ_CHUNK 1
#define MAKE_CHUNK 0

class FileInfo {
public:
	int isComplete() {
		return complete;
	}

private:

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

	void initChunksCountInfo() {
		bytesLeft = fullFileSize % chunkSize;
		if (bytesLeft) bytesLeftFlag = 1;
		if (fullFileSize >= chunkSize) {
			chunksCount = (fullFileSize - bytesLeft) / chunkSize;
			if (bytesLeft) chunksCount++;
		}
		else {
			chunksCount = 1;
		}
	}

	FILE* createMapFile() {

		std::string realFilePath = fileFullPath;
		FILE* file = fopen(mapFilePath.c_str(), "r+b");
		if (nullptr == file) {
			//fclose(file);
			file = fopen(mapFilePath.c_str(), "w+b");
		}
		else {
			return file;
		}
		fwrite(realFilePath.c_str(), realFilePath.size(), 1, file);
		fflush(file);
		bool* chunksMap = nullptr;
		chunksMap = new bool[chunksCount + bytesLeftFlag];


		memset(chunksMap, 0, chunksCount + bytesLeftFlag);
		fwrite(chunksMap, sizeof(bool), chunksCount + bytesLeftFlag, file);
		fflush(file);
		//closeMyFile(file);

		delete[] chunksMap;
		return file;
	}

	int updateMapFile() {

		FILE* mapFile = thisMapFilePointer;
		if (!bindedToMap) {


			int sourceFilePathSize = fileFullPath.length();

			char* filePathInMap = new char[sourceFilePathSize];

			//mapFile = fopen(fileInfo->mapFilePath.c_str(), "rb+");

			fseek(mapFile, 0, SEEK_SET);
			fread(filePathInMap, sourceFilePathSize, 1, mapFile);
			std::string filePathStr = filePathInMap;

			filePathStr.resize(sourceFilePathSize);

			if (filePathStr != fileFullPath.c_str()) {
				std::cout << "Wrong \"fileInfo\" or map file, fileInfo is for " << fileFullPath << " and this map file contains " << filePathInMap << std::endl;
				delete[] filePathInMap;
				return 1;
			}
			delete[] filePathInMap;
			bindedToMap = 1;

		}

		int filePathOffset = 0;
		filePathOffset = fileFullPath.size();

		bool* isInFile = new bool[chunksCount];

		fseek(mapFile, filePathOffset, SEEK_SET);
		fread(isInFile, sizeof(bool), chunksCount, mapFile);

		if (isInFile[chunkInfo->chunkMapPos]) {
			if (!fileContainsChunk(chunkInfo, thisFilePointer)) {
				isInFile[chunkInfo->chunkMapPos] = 0;
				fseek(mapFile, filePathOffset + chunkInfo->chunkMapPos, SEEK_SET);
				fwrite(&isInFile[chunkInfo->chunkMapPos], sizeof(bool), 1, mapFile);
				fflush(mapFile);
				delete[] isInFile;
				return 1;
			}
			std::cout << "Chunk " << chunkInfo->chunkMapPos << " is already in map\n";
			chunksSaved++;
			//delete[] isInFile;
			//return 0;

		}
		else {
			isInFile[chunkInfo->chunkMapPos] = 1;
			fseek(mapFile, filePathOffset + chunkInfo->chunkMapPos, SEEK_SET);
			fwrite(&isInFile[chunkInfo->chunkMapPos], sizeof(bool), 1, mapFile);
			fflush(mapFile);
			chunksSaved++;
		}

		if (chunksSaved == chunksCount) {
			for (size_t i = 0; i < chunksCount; i++)
			{
				if (!isInFile[i]) {
					std::cout << "Chunk " << chunkInfo->chunkMapPos << " is missing from map! Can't finish download! \n";
					delete[] isInFile;
					return 1;
				};
				if (i == chunksCount - 1) {

					complete = 1;

					fflush(mapFile);
					fclose(mapFile);

					remove(mapFilePath.c_str());
					std::cout << "\nMapFile " << mapFilePath << " is removed since the full file is downloaded!\n";


				}
			}
		}

		delete[] isInFile;
		return 0;
	}

	int writeChunkDataToFile() {

		FILE* destFile = thisFilePointer;

		fseek(destFile, chunkSize * chunkInfo->chunkMapPos, SEEK_SET);
		if (chunkInfo->chunkMapPos == chunksCount - 1) {
			fwrite(chunkInfo->chunkRawFileData, bytesLeft, 1, destFile);
			fflush(destFile);
		}
		else {
			fwrite(chunkInfo->chunkRawFileData, chunkSize, 1, destFile);
			fflush(destFile);
		}

		int err = updateMapFile();
		if (!err) delete[] chunkInfo->chunkRawFileData;
		return err;
	}

	char* readChunkData(int chunkMapPos) {


		char* chunkData = new char[chunkSize];
		chunkInfo->chunkMapPos = chunkMapPos;
		fseek(thisFilePointer, chunkSize * chunkInfo->chunkMapPos, SEEK_SET);
		if (chunkInfo->chunkMapPos == chunksCount - 1) {
			fread(chunkData, bytesLeft, 1, thisFilePointer);
		}
		else {
			fread(chunkData, chunkSize, 1, thisFilePointer);
		}

		return chunkData;

	}

	char* readChunkData(ChunkInfo* chunkInfo, int chunkMapPos, FILE* srcFile) {

		char* chunkData = new char[chunkInfo->chunkSize];
		chunkInfo->chunkMapPos = chunkMapPos;
		fseek(srcFile, chunkInfo->chunkSize * chunkInfo->chunkMapPos, SEEK_SET);
		if (chunkInfo->chunkMapPos == chunkInfo->chunksCount - 1) {
			fread(chunkData, chunkInfo->bytesLeft, 1, srcFile);
		}
		else {
			fread(chunkData, chunkInfo->chunkSize, 1, srcFile);
		}

		return chunkData;

	}

public:

	int initForRecv(char* anyRecvdChunk, std::string fileSavePath, std::string tempExt, std::string mapPath, std::string mapExt) {
		if (1 == state) {
			std::cout << "This instance of FileInfo object is already initialized for send\n";
			return 1;
		}
		state = 2;

		chunkInfo = new ChunkInfo(anyRecvdChunk);

		fileFullPath = fileSavePath + chunkInfo->fileName;
		tempFileFullPath = fileFullPath + tempExt;
		fileName = chunkInfo->fileName;
		mapFilePath = mapPath + fileName + mapExt;
		fullFileSize = chunkInfo->sourceFileSize;
		chunkSize = chunkInfo->chunkSize;
		initChunksCountInfo();



		FILE* exists = fopen(fileFullPath.c_str(), "rb");
		if (nullptr != exists) {
			int answ = 0;
			std::cout << "File " << fileFullPath << " already exists, overwrite it? (1/0) ";
			std::cin >> answ;
			if (0 == answ) exit(0);
			fclose(exists);
		}
		exists = fopen(tempFileFullPath.c_str(), "r+b");
		if (nullptr != exists) {
			int answ = 0;
			std::cout << "Resume file " << fileFullPath << " ? (1/0) ";
			std::cin >> answ;
			if (0 == answ) exit(0);
			thisFilePointer = exists;
		}
		else {
			thisFilePointer = fopen(fileFullPath.c_str(), "wb");
			fflush(thisFilePointer);
			fclose(thisFilePointer);
			rename(fileFullPath.c_str(), tempFileFullPath.c_str());
			thisFilePointer = fopen(tempFileFullPath.c_str(), "w+b");
		}

		thisMapFilePointer = createMapFile();

		return 0;
	}

	int initForSend(std::string sourceFileFullPath, int chunkSize) {
		if (2 == state) {
			std::cout << "This instance of FileInfo object is already initialized for receive\n";
			return 1;
		}
		state = 1;

		if (NULL == (thisFilePointer = fopen(sourceFileFullPath.c_str(), "rb"))) {
			std::cout << "CANT OPEN SOURCE FILE!";
			exit(1);
		}

		fileFullPath = sourceFileFullPath;
		fileName = getMyFileName(fileFullPath);
		fullFileSize = getfilesize(fileFullPath.c_str());
		this->chunkSize = chunkSize;
		initChunksCountInfo();

		chunkInfo = new ChunkInfo(fileName, fullFileSize, chunkSize);
		return 0;
	}

	char* createChunk(int mapPos) {

		return chunkInfo->parseChunk(readChunkData(mapPos), MAKE_CHUNK);
	}

	int writeChunk(char* recvdChunk) {

		chunkInfo->parseChunk(recvdChunk, READ_CHUNK);

		return writeChunkDataToFile();
	}

	int resolveFile() {
		FileInfo* fileInfo = this;
		if (!fileInfo->complete) {
			std::cout << "\nFailed to resolve file " << fileInfo->tempFileFullPath << " (file is not complete)\n";
			return 1;
		}
		fflush(fileInfo->thisFilePointer);
		fclose(fileInfo->thisFilePointer);

		std::cout << "\n" << fileInfo->fileName << " full file is downloaded!\n";

		if (0 != rename(fileInfo->tempFileFullPath.c_str(), fileInfo->fileFullPath.c_str())) {
			std::cout << "\nFailed to rename " << fileInfo->tempFileFullPath << " to original: " << fileInfo->fileName << " (maybe file with this name already exists)\n";
		}

		return 0;
	}

	int fileContainsChunk(char* chunk) {
		chunkInfo->parseChunk(chunk, READ_CHUNK);
		fileContainsChunk(chunkInfo, thisFilePointer);
	}

	int fileContainsChunk(ChunkInfo* chunkInfoToCompare, FILE* srcFile) {

		char* chunkData = nullptr;
		int amountToCompare = 0;
		if (chunkInfoToCompare->chunkMapPos == chunkInfoToCompare->chunksCount - 1) {

			amountToCompare = chunkInfoToCompare->bytesLeft;
		}
		else {
			amountToCompare = chunkInfoToCompare->chunkSize;
		}

		chunkData = new char[amountToCompare];

		fseek(srcFile, chunkInfoToCompare->chunkSize * chunkInfoToCompare->chunkMapPos, SEEK_SET);
		int pos = ftell(srcFile);
		fread(chunkData, amountToCompare, 1, srcFile);
		int isEqual = 0;

		for (int i = 0; i < amountToCompare; i++) {
			isEqual = chunkInfoToCompare->chunkRawFileData[i] == chunkData[i] ? 1 : 0;
			if (!isEqual) break;
		}

		delete[] chunkData;

		return isEqual;
	}

	int getChunksCount() {
		return chunksCount;
	}

	~FileInfo() {
		if (nullptr != chunkInfo) {
			delete chunkInfo;
		}
	}

};

