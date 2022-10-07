//
// Created by Andrzej Borucki on 2022-10-07
//

#ifndef TEXTVIEWER_CACHEMAX_H
#define TEXTVIEWER_CACHEMAX_H

#include <unordered_map>

class CacheMax {
    std::unordered_map<int64_t, int> ma;
public:
    int get(int64_t start);
    void add(int64_t start, int dlen);
};


#endif //TEXTVIEWER_CACHEMAX_H
