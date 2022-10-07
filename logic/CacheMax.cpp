//
// Created by andrzej on 10/7/22.
//

#include <cstdint>
#include "CacheMax.h"

int CacheMax::get(int64_t start) {
    auto it = ma.find(start);
    if (it == ma.end())
        return -1;
    else
        return it->second;
}

void CacheMax::add(int64_t start, int dlen) {
    ma[start] = dlen;
}
