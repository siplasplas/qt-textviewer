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
    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    EXPECT_GT(vexpect.size(), 0);
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}

TEST(Wrap, backward_short) {
    string content = makeContent("../test/shortwrap.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 7;
    vl.screenLineCount = 20;
    vl.wrapMode = 1;
    vl.lo->gotoFromBegin(0);
    auto vexpect = vl.lines();
    vl.screenLineCount = vexpect.size()-1;
    vl.lo->gotoProportional(content.length());
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size()-1, vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()-1); i++)
        EXPECT_EQ(vtest[i], vexpect[i+1]);
}

TEST(Wrap, backward_short_empt) {
    string content = makeContent("../test/emshortwrap.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 7;
    vl.screenLineCount = 20;
    vl.wrapMode = 1;
    vl.lo->gotoFromBegin(0);
    auto vexpect = vl.lines();
    vl.screenLineCount = vexpect.size()-1;
    vl.lo->gotoProportional(content.length());
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size()-1, vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()-1); i++)
        EXPECT_EQ(vtest[i], vexpect[i+1]);
}

TEST(Wrap, backward_mid) {
    string content = makeContent("../test/midlines.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 100;
    vl.screenLineCount = 20;
    vl.maxLineLen = 1000;
    vl.wrapMode = 1;
    vl.lo->gotoFromBegin(0);
    auto vexpect = vl.lines();
    vl.screenLineCount = vexpect.size()-1;
    vl.lo->gotoProportional(content.length());
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size()-1, vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()-1); i++)
        EXPECT_EQ(vtest[i], vexpect[i+1]);
}

TEST(Wrap, backward_short_any) {
    string content = makeContent("../test/shortwrap.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 7;
    vl.screenLineCount = 20;
    vl.wrapMode = 1;
    vl.lo->gotoFromBegin(0);
    auto vexpect = vl.lines();
    vl.screenLineCount = vexpect.size()-2;
    vl.lo->gotoProportional(28);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size()-2, vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()-1); i++)
        EXPECT_EQ(vtest[i], vexpect[i+1]);
}

TEST(Wordwrap, words) {
    string content = makeContent("../test/wordwrap.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 20;
    vl.wrapMode = 2;
    vl.maxLineLen = 10000;
    auto vexpect = makeExpect("../expect/wordwrap_first11ww10.txt");
    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}
