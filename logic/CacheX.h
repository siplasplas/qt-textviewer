//
// Created by Andrzej Borucki on 2022-10-02
//

//for horizontal scrolling

#ifndef TEXTVIEWER_CACHEX_H
#define TEXTVIEWER_CACHEX_H

#include <unordered_map>
#include <cstdint>

class CacheX {
    struct InfoX {
        int beginX;
        int rawBeginX;
        InfoX() = default;
        InfoX(int beginX,int rawBeginX):beginX(beginX),rawBeginX(rawBeginX){}
//        InfoX(const InfoX& other) {
//            beginX = other.beginX;
//            rawBeginX = other.rawBeginX;
//        }
//        InfoX& operator=(const InfoX& other) {
//            beginX = other.beginX;
//            rawBeginX = other.rawBeginX;
//            return *this;
//        }
    };
    std::unordered_map<int64_t, InfoX> ma;
public:
    std::pair<int,int> getPrevAndDelta(int64_t startLine, int beginX);
    void add(int64_t startLine, int beginX, int rawBeginX);
};


#endif //TEXTVIEWER_CACHEX_H
