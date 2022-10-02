//
// Created by andrzej on 10/2/22.
//
#include "CacheX.h"

using namespace std;

pair<int, int> CacheX::getPrevAndDelta(int64_t startLine, int beginX) {
    auto it = ma.find(startLine);
    if (it == ma.end())
        return make_pair(0,0);
    else {
        auto &infoX = it->second;
        return make_pair(infoX.rawBeginX, beginX - infoX.beginX);
    }
}

void CacheX::add(int64_t startLine, int beginX, int rawBeginX) {
    InfoX infoX(beginX, rawBeginX);
    ma[startLine] = infoX;
}
