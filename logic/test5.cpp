//
// Created by Andrzej Borucki on 2022-10-02
//
#include "ViewLogic.h"
#include "Util.h"
#include "gtest/gtest.h"

using namespace std;
using namespace vl;

TEST(beginX, UnicodeX10) {
    string content = makeContent("../test/utf8.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 10;
    vl.wrapMode = 0;
    vl.maxLineLen = 1000;
    auto vexpect = makeExpect("../expect/utf8_first10w10X10.txt");
    vl.lo->gotoFromBegin(0, 10);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}

TEST(beginX, UnicodeX12) {
    string content = makeContent("../test/utf8.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 10;
    vl.wrapMode = 0;
    vl.maxLineLen = 1000;
    auto vexpect = makeExpect("../expect/utf8_first10w10X12.txt");
    vl.lo->gotoFromBegin(0,12);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}

TEST(beginX, UnicodeX40) {
    string content = makeContent("../test/utf8.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 10;
    vl.wrapMode = 0;
    vl.maxLineLen = 1000;
    auto vexpect = makeExpect("../expect/utf8_first10w10X40.txt");
    vl.lo->gotoFromBegin(0,40);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}

TEST(beginX, cache) {
    string content = makeContent("../test/utf8.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 10;
    vl.wrapMode = 0;
    vl.maxLineLen = 1000;
    vl.minLineToCacheX = 0;
    auto vexpect0 = makeExpect("../expect/utf8_first10w10.txt");
    auto vexpect10 = makeExpect("../expect/utf8_first10w10X10.txt");
    auto vexpect12 = makeExpect("../expect/utf8_first10w10X12.txt");
    auto vexpect40 = makeExpect("../expect/utf8_first10w10X40.txt");

    vl.lo->gotoFromBegin(0,0);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect0.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect0.size()); i++)
        EXPECT_EQ(vtest[i], vexpect0[i]);

    vl.lo->gotoFromBegin(0,10);
    vtest = vl.lines();
    EXPECT_EQ(vexpect10.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect10.size()); i++)
        EXPECT_EQ(vtest[i], vexpect10[i]);

    vl.lo->gotoFromBegin(0,12);
    vtest = vl.lines();
    EXPECT_EQ(vexpect12.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect12.size()); i++)
        EXPECT_EQ(vtest[i], vexpect12[i]);
    //only from cache
    vl.lo->gotoFromBegin(0,12);
    vtest = vl.lines();
    EXPECT_EQ(vexpect12.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect12.size()); i++)
        EXPECT_EQ(vtest[i], vexpect12[i]);

    vl.lo->gotoFromBegin(0,40);
    vtest = vl.lines();
    EXPECT_EQ(vexpect40.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect40.size()); i++)
        EXPECT_EQ(vtest[i], vexpect40[i]);

    vl.lo->gotoFromBegin(0,12);
    vtest = vl.lines();
    EXPECT_EQ(vexpect12.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect12.size()); i++)
        EXPECT_EQ(vtest[i], vexpect12[i]);

    vl.lo->gotoFromBegin(0,10);
    vtest = vl.lines();
    EXPECT_EQ(vexpect10.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect10.size()); i++)
        EXPECT_EQ(vtest[i], vexpect10[i]);

    vl.lo->gotoFromBegin(0,0);
    vtest = vl.lines();
    EXPECT_EQ(vexpect0.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect0.size()); i++)
        EXPECT_EQ(vtest[i], vexpect0[i]);
}
