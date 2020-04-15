#pragma once
#include <map>
#define CHUNKINFO_INIT 2
#define READ_CHUNK 1
#define MAKE_CHUNK 0

class ChunkInfo {

private:
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

	ChunkInfo(std::string fileName, int sourceFileSize, int chunkSize) : fileName(fileName), sourceFileSize(sourceFileSize), chunkSize(chunkSize) {
		sendReady = 1;
		initChunksCountInfo();
	}

	ChunkInfo() {

	}

	void initChunksCountInfo() {
		bytesLeft = sourceFileSize % chunkSize;
		chunksCount = (sourceFileSize - bytesLeft) / chunkSize;
		if (sourceFileSize < chunkSize) {
			chunksCount = 0;
		}
		else {
			chunksCount = (sourceFileSize - bytesLeft) / chunkSize;
		}
	}

	int isSendReady() {
		if (chunkSize && sourceFileSize && (fileName != "")) {
			sendReady = 1;
		}
		return sendReady;
	}


};


char* parseChunk(ChunkInfo* chunkInfo, char* chunkMsg, int flag) {

	//(HEAD) chunkMapPos(int), sourceFileSize(int), chunkSize(int), filename(string), \n , *File data*

	int chunkDataOffset = 0;
	char headTerminator = '\n';

	if (MAKE_CHUNK == flag) {
		if (!chunkInfo->isSendReady()) { std::cout << std::endl << "This ChunkInfo instance initialized with no parameters!" << std::endl; }


		int chunkDataOffset = 0;

		char* msg = new char[chunkInfo->chunkSize + 300];


		memcpy(msg + chunkDataOffset, static_cast<void*>(&chunkInfo->chunkMapPos), sizeof(int));
		chunkDataOffset += sizeof(int);
		memcpy(msg + chunkDataOffset, static_cast<void*>(&chunkInfo->sourceFileSize), sizeof(int));
		chunkDataOffset += sizeof(int);
		memcpy(msg + chunkDataOffset, static_cast<void*>(&chunkInfo->chunkSize), sizeof(int));
		chunkDataOffset += sizeof(int);
		memcpy(msg + chunkDataOffset, chunkInfo->fileName.c_str(), chunkInfo->fileName.size());
		chunkDataOffset += chunkInfo->fileName.size();



		msg[chunkDataOffset] = headTerminator;
		chunkDataOffset++;
		chunkInfo->chunkDataOffset = chunkDataOffset;

		memcpy(msg + chunkDataOffset, chunkMsg, chunkInfo->chunkSize);
		chunkInfo->chunkRawFileData = chunkMsg;
		return msg;
	}


	if (READ_CHUNK == flag || CHUNKINFO_INIT == flag) {

		//(HEAD) chunkMapPos(int), sourceFileSize(int), chunkSize(int), filename(string), \n , *File data*

		memcpy(static_cast<void*>(&chunkInfo->chunkMapPos), chunkMsg + chunkDataOffset, sizeof(int)); //chunkMapPos
		chunkDataOffset += sizeof(int);

		memcpy(static_cast<void*>(&chunkInfo->sourceFileSize), chunkMsg + chunkDataOffset, sizeof(int)); //sourcesize
		chunkDataOffset += sizeof(int);

		memcpy(static_cast<void*>(&chunkInfo->chunkSize), chunkMsg + chunkDataOffset, sizeof(int)); //fileChunkSize
		chunkDataOffset += sizeof(int);

		for (; chunkDataOffset < chunkInfo->chunkSize; chunkDataOffset++) {
			if (headTerminator == chunkMsg[chunkDataOffset]) { chunkDataOffset++; break; };
			chunkInfo->fileName += chunkMsg[chunkDataOffset];
		}
		chunkInfo->chunkDataOffset = chunkDataOffset;

		chunkInfo->initChunksCountInfo();

		if (READ_CHUNK == flag)
		{
			char* chunkData = new char[chunkInfo->chunkSize];
			memcpy(chunkData, chunkMsg + chunkInfo->chunkDataOffset, chunkInfo->chunkSize);
			chunkInfo->chunkRawFileData = chunkData;
			return chunkInfo->chunkRawFileData;
		}
		return chunkMsg;

	}



}

void writeChunkDataToFile(ChunkInfo* chunkInfo, FILE* dest) {

	fseek(dest, chunkInfo->chunkSize * chunkInfo->chunkMapPos, SEEK_SET);
	if (chunkInfo->chunkMapPos == chunkInfo->chunksCount) {
		fwrite(chunkInfo->chunkRawFileData, chunkInfo->bytesLeft, 1, dest);
		fflush(dest);
	}
	else {
		fwrite(chunkInfo->chunkRawFileData, chunkInfo->chunkSize, 1, dest);
		fflush(dest);
	}
}


char* createChunkData(ChunkInfo* chunkInfo, int chunkMapPos, FILE* srcFile) {

	char* chunkData = new char[chunkInfo->chunkSize];
	chunkInfo->chunkMapPos = chunkMapPos;
	fseek(srcFile, chunkInfo->chunkSize * chunkInfo->chunkMapPos, SEEK_SET);
	if (chunkInfo->chunkMapPos == chunkInfo->chunksCount) {
		fread(chunkData, chunkInfo->bytesLeft, 1, srcFile);
	}
	else {
		fread(chunkData, chunkInfo->chunkSize, 1, srcFile);
	}

	return chunkData;


}

int fileContainsChunk(ChunkInfo* chunkInfoToCompare, FILE* srcFile) {

	char* chunkData = new char[chunkInfoToCompare->chunkSize];

	fseek(srcFile, chunkInfoToCompare->chunkSize * chunkInfoToCompare->chunkMapPos, SEEK_SET);
	int pos = ftell(srcFile);
	fread(chunkData, chunkInfoToCompare->chunkSize, 1, srcFile);
	int isEqual = 0;

	for (int i = 0; i < chunkInfoToCompare->chunkSize; i++) {
		isEqual = chunkInfoToCompare->chunkRawFileData[i] == chunkData[i] ? 1 : 0;
		if (!isEqual) break;
	}

	delete[] chunkData;

	return isEqual;
}


FILE* openMyFile(const char* path, int mode = 0) {
	FILE* file;

	std::string fileMode = "";

	if (mode == 0) fileMode = "rb";
	if (mode == 1) fileMode = "wb";

	if ((file = fopen(path, fileMode.c_str())) == NULL) {
		std::cout << "CANT OPEN SOURCE FILE!";
		exit(1);
	}

	return file;
}


void closeMyFile(FILE* file) {
	fclose(file);
}

FILE* createTempFile(const char* path, ChunkInfo* chunkInfo, int* fileHeadOffset) { //WIP
	FILE* file = openMyFile(path, 1);

	int chunksCount = chunkInfo->chunksCount;

	char fileHead[sizeof(int)];

	memcpy(fileHead, static_cast<void*>(&chunksCount), sizeof(int));
	fwrite(fileHead, sizeof(int), 1, file);
	*fileHeadOffset = sizeof(int);

	closeMyFile(file);

	return file;
}




int getfilesize(const char* path) {
	FILE* source;

	if ((source = fopen(path, "rb")) == NULL) {
		std::cout << "CANT OPEN FILE";
		return -1;
	}

	fseek(source, 0, SEEK_END);
	int filesize = ftell(source);
	fclose(source);
	return filesize;
}


int readFileData(const char* path, int filesize, char*& buffer) {

	FILE* source;

	if ((source = fopen(path, "rb")) == NULL) {
		std::cout << "CANT OPEN SOURCE FILE!";
		return -1;
	}

	fread(buffer, filesize, 1, source);
	fclose(source);
	return 0;
};

void writeFileData(const char* path, int filesize, char* buffer, int from = 0) {

	std::cout << std::endl << "FROM IS: " << from << std::endl;
	FILE* dest;

	if ((dest = fopen(path, "wb")) == NULL) {
		std::cout << "CANT OPEN/CREATE DEST FILE!";
		return;
	}

	fwrite(buffer + from, filesize, 1, dest);
	fflush(dest);
	fclose(dest);
};

int copyFileData(const char* srcPath, const char* destPath) {

	int sourcesize = getfilesize(srcPath);
	char* buffer = new char[sourcesize];

	readFileData(srcPath, sourcesize, buffer);
	writeFileData(destPath, sourcesize, buffer);

	delete[] buffer;

	return 0;
}

std::string getMyFileName(std::string path) {
	std::size_t found = path.find_last_of("/\\");
	return path.substr(found + 1).c_str();
}


int sendFile(const char* path, SocketsAPI* destSocket) {

	int sourcesize = getfilesize(path);
	//const char* filename = getMyFileName(path).c_str();
	std::cout << std::endl << "sourcesize is " << sourcesize << std::endl;
	//destSocket.setBufferSize(sourcesize);
	char* buffer = new char[sourcesize];
	if (!0 == readFileData(path, sourcesize, buffer)) {
		return -1;
	};

	std::string str{ "xxxxxxxxxxxx" };
	std::to_chars(str.data(), str.data() + str.size(), sourcesize);

	std::string msg = std::to_string(sourcesize) + "\n" + getMyFileName(path).c_str() + "\n\n";
	auto msgChars = new char[destSocket->getBufferSize()];
	//destSocket.setBufferSize(5000000);
	memcpy(msgChars, msg.c_str(), sizeof(msg));
	memcpy(msgChars + msg.length(), buffer, sourcesize);
	//send(server, buffer, size, 0);
	destSocket->sendMsg(msgChars, sourcesize + sizeof(msg));
	delete[] buffer;
	delete[] msgChars;
}


char* chunkParser(ChunkInfo* chunkInfo, char* chunkMsg, int flag = 0) {

	//(HEAD) chunkMapPos(int), sourcesize(int), chunkSize(int), Filename(string), \n *File data*

	int chunkDataOffset = 0;
	char headTerminator = '\n';

	if (MAKE_CHUNK == flag) {
		if (!chunkInfo->isSendReady()) { std::cout << std::endl << "This ChunkInfo instance initialized with no parameters!" << std::endl; }


		int chunkDataOffset = 0;

		char* msg = new char[chunkInfo->chunkSize + 300];


		memcpy(msg + chunkDataOffset, static_cast<void*>(&chunkInfo->chunkMapPos), sizeof(int));
		chunkDataOffset += sizeof(int);
		memcpy(msg + chunkDataOffset, static_cast<void*>(&chunkInfo->sourceFileSize), sizeof(int));
		chunkDataOffset += sizeof(int);
		memcpy(msg + chunkDataOffset, static_cast<void*>(&chunkInfo->chunkSize), sizeof(int));
		chunkDataOffset += sizeof(int);
		memcpy(msg + chunkDataOffset, chunkInfo->fileName.c_str(), chunkInfo->fileName.size());
		chunkDataOffset += chunkInfo->fileName.size();


		//(HEAD) chunkMapPos(int), sourcesize(int), chunkSize(int), filename(string), \n *File data*
		msg[chunkDataOffset] = headTerminator;
		chunkDataOffset++;
		chunkInfo->chunkDataOffset = chunkDataOffset;

		memcpy(msg + chunkDataOffset, chunkMsg, chunkInfo->chunkSize);

		return msg;
	}


	if (READ_CHUNK == flag) {
		if ("" == chunkInfo->fileName) {
			memcpy(static_cast<void*>(&chunkInfo->chunkMapPos), chunkMsg + chunkDataOffset, sizeof(int)); //sourcesize
			chunkDataOffset += sizeof(int);

			memcpy(static_cast<void*>(&chunkInfo->sourceFileSize), chunkMsg + chunkDataOffset, sizeof(int)); //fileChunkSize
			chunkDataOffset += sizeof(int);

			memcpy(static_cast<void*>(&chunkInfo->chunkSize), chunkMsg + chunkDataOffset, sizeof(int)); //fileChunkSize
			chunkDataOffset += sizeof(int);

			for (; chunkDataOffset < chunkInfo->chunkSize; chunkDataOffset++) {
				if (headTerminator == chunkMsg[chunkDataOffset]) chunkDataOffset++; break;
				chunkInfo->fileName += chunkMsg[chunkDataOffset];
				chunkDataOffset++;
			}
			chunkInfo->chunkDataOffset = chunkDataOffset;
		}

		char* chunkData = new char[chunkInfo->chunkSize];

		memcpy(chunkData, chunkMsg + chunkInfo->chunkDataOffset, chunkInfo->chunkSize);

		return chunkData;

	}

}


