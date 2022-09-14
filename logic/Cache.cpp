//
// Created by andrzej on 9/14/22.
//

#include <cstdint>
#include "Cache.h"

int Cache::getLineLen(int64_t startpos) {
    auto it = ma1.find(startpos);
    if (it == ma1.end())
        return 0;
    else
        return it->second+1;
}

void Cache::put(int64_t startpos, int len) {
    len--;
    ma1[startpos] = len;
    ma2[startpos+len] = len;
}

int Cache::getPrevLineLen(long startpos) {
    auto it = ma2.find(startpos);
    if (it == ma2.end())
        return 0;
    else
        return it->second;
}

void Cache::putPrev(int64_t startpos, int len) {
    ma2[startpos] = len;
    ma1[startpos-len] = len;
}
