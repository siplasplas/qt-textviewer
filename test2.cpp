//
// Created by Andrzej Borucki on 2022-09-11
//

#include "ViewLogic.h"
#include "Util.h"
#include "gtest/gtest.h"

using namespace std;
using namespace vl;

TEST (Wrap, forward) {
    string content = makeContent("../test/midlines.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 20;
    vl.maxLineLen = 1000;
    vl.wrapMode = 1;
    auto vexpect = makeExpect("../expect/midlines_first20wrap10.txt");
    auto vtest = vl.linesFromBeginScreen(0);
    EXPECT_GT(vexpect.size(), 0);
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}