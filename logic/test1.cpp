//
// Created by Andrzej Borucki on 2022-09-11
//

#include "ViewLogic.h"
#include "Util.h"
#include "gtest/gtest.h"

using namespace std;
using namespace vl;

TEST (Width, empties) {
    string content = makeContent("../test/empties.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 8;
    vl.screenLineCount = 20;
    auto vexpect = makeExpect("../expect/empties_first20w8.txt");
    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}

TEST(Back, positions) {
    string content = makeContent("../test/empties.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 8;
    vl.screenLineCount = 50; //enough for all lines
    vl.lo->gotoFromBegin(0);
    auto vexpect = vl.lines();
    vl.screenLineCount = 0;
    vl.lo->gotoProportional(content.length());
    EXPECT_EQ(vl.lo->li->offset, content.length());
    for (int i=1; i<vexpect.size(); i++) {
        vl.screenLineCount = i;
        vl.lo->gotoProportional(content.length());
        EXPECT_EQ(vl.lo->li->offset, vexpect.lines->at(vexpect.size()-i).li->offset);
    }
}

TEST(Back, lines) {
    string content = makeContent("../test/empties.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 8;
    vl.screenLineCount = 50; //enough for all lines
    vl.lo->gotoFromBegin(0);
    auto vexpect = vl.lines();
    vl.screenLineCount = vexpect.size();
    vl.lo->gotoProportional(content.length());
    auto vtest =  vl.lines();
    EXPECT_EQ(vtest, vexpect);
}

TEST (Max, forward) {
    string content = makeContent("../test/max.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 100;
    vl.screenLineCount = 21;
    vl.maxLineLen = 40;
    auto vexpect = makeExpect("../expect/max_first16m40w100.txt");
    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    EXPECT_GT(vexpect.size(), 0);
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}

TEST(Max, backward) {
    string content = makeContent("../test/max.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 100;
    vl.screenLineCount = 21;
    vl.maxLineLen = 40;
    vl.lo->gotoFromBegin(0);
    auto vexpect = vl.lines();
    vl.screenLineCount = vexpect.size();
    vl.lo->gotoProportional(content.length());
    auto vtest = vl.lines();
    EXPECT_GT(vexpect.size(), 0);
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}
