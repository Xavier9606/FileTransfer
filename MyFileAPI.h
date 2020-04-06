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