#pragma once

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

int sendFile(const char* path, SocketsAPI destSocket) {

    int sourcesize = getfilesize(path);
    std::cout << std::endl << "sourcesize is " << sourcesize << std::endl;
    destSocket.setBufferSize(sourcesize);
    char* buffer = new char[sourcesize];
    if (!0 == readFileData(path, sourcesize, buffer)) {
        return -1;
    };

    std::string str{ "xxxxxxxxxxxx" };
    std::to_chars(str.data(), str.data() + str.size(), sourcesize);

    std::string msg = std::to_string(sourcesize) + "\n";
    auto msgChars = new char[5000000];
    destSocket.setBufferSize(5000000);
    memcpy(msgChars, msg.c_str(), sizeof(msg));
    memcpy(msgChars + msg.length(), buffer, sourcesize);
    destSocket.sendMsg(msgChars, sourcesize);
    delete[] buffer;
    delete[] msgChars;
}

const char* getMyFileName(std::string path) {
    std::size_t found = path.find_last_of("/\\");
    return path.substr(found + 1).c_str();
}
