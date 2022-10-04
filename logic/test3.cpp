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
    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}

TEST(Unicode, wrap) {
    string content = makeContent("../test/utf8.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 20;
    vl.wrapMode = 1;
    vl.maxLineLen = 1000;
    auto vexpect = makeExpect("../expect/utf8_first20wrap10.txt");
    vl.lo->gotoFromBegin(0);
    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}

TEST(Unicode, maxline) {
    string content = makeContent("../test/utf8.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 100;
    vl.screenLineCount = 40;
    vl.wrapMode = 0;
    vl.maxLineLen = 10;
    auto vexpect = makeExpect("../expect/utf8_first40m10.txt");
    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}

TEST(Tabs, nowrap) {
    string content = makeContent("../test/tabs.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 100;
    vl.screenLineCount = 10;
    vl.wrapMode = 0;
    vl.maxLineLen = 10000;
    vl.maxTabW = 4;
    auto vexpect = makeExpect("../expect/tabs.txt");
    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}

TEST(Tabs, wrap) {
    string content = makeContent("../test/tabs.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 6;
    vl.screenLineCount = 10;
    vl.wrapMode = 1;
    vl.maxLineLen = 10000;
    vl.maxTabW = 4;
    auto vexpect = makeExpect("../expect/tabs.wrap.txt");
    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}

TEST(Tabs, wordwrap) {
    string content = makeContent("../test/tabs.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 6;
    vl.screenLineCount = 10;
    vl.wrapMode = 2;
    vl.maxLineLen = 10000;
    vl.maxTabW = 4;
    auto vexpect = makeExpect("../expect/tabs.wordwrap.txt");
    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}
