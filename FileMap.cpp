//
// Created by Andrzej Borucki on 2022-09-13
//
#include "FileMap.h"

using namespace std;
using namespace boost::interprocess;

FileMap::~FileMap() {
    closeFile();
}

bool FileMap::openFile(const char *fileName) {
    closeFile();
    strFileName = fileName;
    try {
        mapFile = new file_mapping(fileName, read_only);
    } catch(boost::interprocess::interprocess_exception) {
        return false;
    }
    region = new mapped_region(*mapFile, read_only);
    addr = (char *)region->get_address();
    fileSize = (int64_t)region->get_size();
    return true;
}

void FileMap::closeFile() {
    delete region;
    region = nullptr;
    delete mapFile;
    mapFile = nullptr;
    addr = nullptr;
}
