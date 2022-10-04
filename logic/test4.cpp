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

    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    auto orig = vtest;
    EXPECT_EQ(vtest, orig);
    vl.scrollDown(vtest);
    vl.scrollUp(vtest);
    EXPECT_EQ(vtest, orig);
    vl.scrollPageDown(vtest);
    vl.scrollPageUp(vtest);
    EXPECT_EQ(vtest, orig);
}

TEST(Scroll, wrap) {
    string content = makeContent("../test/midlines.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 6;
    vl.screenLineCount = 3;
    vl.wrapMode = 1;

    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    auto orig = vtest;
    EXPECT_EQ(vtest, orig);
    vl.scrollDown(vtest);
    vl.scrollUp(vtest);
    EXPECT_EQ(vtest.infos->size(), orig.infos->size());
    for (int i=0; i<std::min(vtest.infos->size(), orig.infos->size()); i++)
        EXPECT_EQ(*vtest.infos->at(i), *orig.infos->at(i));
    EXPECT_EQ(vtest.lines->size(), orig.lines->size());
    for (int i=0; i<std::min(vtest.lines->size(), orig.lines->size()); i++) {
        auto lineA = vtest.lines->at(i);
        auto lineB = orig.lines->at(i);
        EXPECT_EQ(lineA.wrapIndex, lineB.wrapIndex);
        EXPECT_EQ(*lineA.li, *lineB.li);
        EXPECT_EQ(lineA.text, lineB.text);
    }
    EXPECT_EQ(vtest, orig);
    //vl.scrollPageDown(vtest);
    //vl.scrollPageUp(vtest);
    vl.scrollNDown(2,vtest);
    vl.scrollNUp(2, vtest);
    EXPECT_EQ(vtest, orig);
}