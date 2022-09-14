//
// Created by Andrzej Borucki on 2022-09-14
//

#ifndef TEXTVIEWER_CACHE_H
#define TEXTVIEWER_CACHE_H

#include <unordered_map>

class Cache {
    std::unordered_map<int64_t, int> ma1;
    std::unordered_map<int64_t, int> ma2;
public:
    int getLineLen(long startpos);
    void put(int64_t startpos, int len);
    int getPrevLineLen(long startpos);
    void putPrev(int64_t startpos, int len);
};


#endif //TEXTVIEWER_CACHE_H
