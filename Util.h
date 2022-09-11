//
// Created by andrzej on 2022-08-25
//

#ifndef VIEWER_UTIL_H
#define VIEWER_UTIL_H

#include <string>
#include <vector>

std::vector<std::string> readLines(std::string filename);
std::string makeContent(std::string filename);
std::vector<std::wstring> makeExpect(std::string filename);

#endif //VIEWER_UTIL_H
