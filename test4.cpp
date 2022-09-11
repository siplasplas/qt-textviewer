//
// Created by Andrzej Borucki on 2022-09-11
//
//Scroll

#include "ViewLogic.h"
#include "gtest/gtest.h"
#include "Util.h"

using namespace std;
using namespace vl;

TEST(Scroll, width) {
    string content = makeContent("../test/midlines.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 6;
    vl.screenLineCount = 3;
    vl.wrapMode = 0;

    auto vtest = vl.lines(vl.fileSize);
    auto orig = vtest;
    EXPECT_EQ(vtest, orig);
    vl.scrollDown(vtest);
    vl.scrollDown(vtest);
    //vl.scrollUp(vtest);
    //vl.scrollUp(vtest);
    EXPECT_EQ(vtest, orig);
    vl.scrollPageDown(vtest);
    vl.scrollPageUp(vtest);
    EXPECT_EQ(vtest, orig);
}

TEST(Scroll, wrap) {
    string content = makeContent("../test/midlines.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 5;
    vl.wrapMode = 1;

    auto vtest = vl.lines(0);
    auto orig = vtest;
    EXPECT_EQ(vtest, orig);
    vl.scrollDown(vtest);
    vl.scrollUp(vtest);
    EXPECT_EQ(vtest, orig);
    vl.scrollPageDown(vtest);
    vl.scrollPageUp(vtest);
    EXPECT_EQ(vtest, orig);
}
