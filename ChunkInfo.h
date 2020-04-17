#pragma once

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
		//chunkRawFileData = new char[this->chunkSize+300];
	}

	ChunkInfo(char* chunkMsg) {
		parseChunk(chunkMsg, 2);
		//chunkRawFileData = new char[this->chunkSize+300];
	}

	void initChunksCountInfo() {
		bytesLeft = sourceFileSize % chunkSize;
		if (sourceFileSize >= chunkSize) {
			chunksCount = (sourceFileSize - bytesLeft) / chunkSize;
			if (bytesLeft) chunksCount++;
		}
		else {
			chunksCount = 1;
		}


	}

	int isSendReady() {
		if (chunkSize && sourceFileSize && (fileName != "")) {
			sendReady = 1;
		}
		return sendReady;
	}

	char* parseChunk(char* chunkMsg, int flag) {

		//(HEAD) chunkMapPos(int), sourceFileSize(int), chunkSize(int), filename(string), \n , *File data*

		char headTerminator = '\n';

		if (MAKE_CHUNK == flag) {
			if (!this->isSendReady()) { std::cout << std::endl << "This ChunkInfo instance have not enough parameters to be send!" << std::endl; return nullptr; }



			char* msg = nullptr;
			chunkDataOffset = 0;

			if (chunkMapPos == chunksCount - 1) {
				msg = new char[bytesLeft + 300];
			}
			else {
				msg = new char[chunkSize + 300];
			}



			memcpy(msg + chunkDataOffset, static_cast<void*>(&this->chunkMapPos), sizeof(int)); //chunkMapPos
			chunkDataOffset += sizeof(int);
			memcpy(msg + chunkDataOffset, static_cast<void*>(&this->sourceFileSize), sizeof(int)); //sourcesize
			chunkDataOffset += sizeof(int);
			memcpy(msg + chunkDataOffset, static_cast<void*>(&this->chunkSize), sizeof(int)); //fileChunkSize
			chunkDataOffset += sizeof(int);
			memcpy(msg + chunkDataOffset, this->fileName.c_str(), this->fileName.size());	//fileName
			chunkDataOffset += this->fileName.size();



			msg[chunkDataOffset] = headTerminator;
			chunkDataOffset++;



			if (chunkMapPos == chunksCount - 1) {
				memcpy(msg + chunkDataOffset, chunkMsg, bytesLeft);
			}
			else {
				memcpy(msg + chunkDataOffset, chunkMsg, chunkSize);
			}

			delete[] chunkMsg;
			chunkRawFileData = msg;
			return msg;
		}


		if (READ_CHUNK == flag || CHUNKINFO_INIT == flag) {

			//(HEAD) chunkMapPos(int), sourceFileSize(int), chunkSize(int), filename(string), \n , *File data*

			memcpy(static_cast<void*>(&this->chunkMapPos), chunkMsg, sizeof(int)); //chunkMapPos

			if (!chunkDataOffset) {

				chunkDataOffset += sizeof(int);

				memcpy(static_cast<void*>(&this->sourceFileSize), chunkMsg + chunkDataOffset, sizeof(int)); //sourcesize
				chunkDataOffset += sizeof(int);

				memcpy(static_cast<void*>(&this->chunkSize), chunkMsg + chunkDataOffset, sizeof(int)); //fileChunkSize
				chunkDataOffset += sizeof(int);

				for (; chunkDataOffset < this->chunkSize; chunkDataOffset++) {
					if (headTerminator == chunkMsg[chunkDataOffset]) { chunkDataOffset++; break; };		//fileName
					this->fileName += chunkMsg[chunkDataOffset];
				}
				this->chunkDataOffset = chunkDataOffset;

				this->initChunksCountInfo();
			}
			if (READ_CHUNK == flag)
			{
				char* chunkData = nullptr;

				if (chunkMapPos == chunksCount - 1) {
					chunkData = new char[bytesLeft];
					memcpy(chunkData, chunkMsg + chunkDataOffset, bytesLeft);
				}
				else {
					chunkData = new char[chunkSize];
					memcpy(chunkData, chunkMsg + chunkDataOffset, chunkSize);
				}

				chunkRawFileData = chunkData;
				return chunkRawFileData;
			}
			return chunkMsg;

		}

	}

	~ChunkInfo() {
		if (nullptr != chunkRawFileData) {
			//delete[] chunkRawFileData;
		}
	}
};