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
    auto vtest = vl.linesFromBeginScreen(0);
    EXPECT_EQ(vexpect.size(), vtest.size());
    for (int i=0; i<min(vtest.size(),vexpect.size()); i++)
        EXPECT_EQ(vtest[i], vexpect[i]);
}

TEST(Back, positions) {
    string content = makeContent("../test/empties.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 8;
    vl.screenLineCount = 50; //enough forall lines
    auto vexpect = vl.linesFromBeginScreen(0);
    vl.screenLineCount = 0;
    EXPECT_EQ(vl.getBeginPos(content.length()).li->offset, content.length());
    for (int i=1; i<vexpect.size(); i++) {
        vl.screenLineCount = i;
        EXPECT_EQ(vl.getBeginPos(content.length()).li->offset, vexpect.lines->at(vexpect.size()-i).li->offset);
    }
}