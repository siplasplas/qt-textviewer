//
// Created by Andrzej Borucki on 2022-09-11
//

#include "ViewLogic.h"
#include "Util.h"
#include "gtest/gtest.h"

using namespace std;
using namespace vl;

TEST(Unicode, width) {
    string content = makeContent("../test/utf8.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 10;
    vl.wrapMode = 0;
    vl.maxLineLen = 1000;
    auto vexpect = makeExpect("../expect/utf8_first10w10.txt");
    auto vtest = vl.linesFromBeginScreen(0);
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}
