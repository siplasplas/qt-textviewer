//
// Created by Andrzej Borucki on 2022-09-13
//
#ifndef VIEWER_FILEMAP_H
#define VIEWER_FILEMAP_H

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "logic/ViewLogic.h"

class FileMap {
    std::string strFileName;
    boost::interprocess::file_mapping *mapFile = nullptr;
    boost::interprocess::mapped_region *region = nullptr;
    char *addr = nullptr;
    int64_t fileSize = 0;
public:
    ~FileMap();
    bool openFile(const char *fileName);
    void closeFile();
    int64_t getFileSize(){ return fileSize; }
    char* getAddr() {return addr; }
};

#endif //VIEWER_FILEMAP_H
