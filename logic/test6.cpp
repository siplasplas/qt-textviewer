//
// Created by Andrzej Borucki on 2022-10-04
//
#include "ViewLogic.h"
#include "Util.h"
#include "gtest/gtest.h"

using namespace std;
using namespace vl;

TEST(locatePosition, whole) {
    const string names[] = {"shortwrap.txt", "utf8.txt", "midlines.txt"};
    for (int k=0; k<3; k++) {
        string content = makeContent(string("../test/")+names[k]);
        ViewLogic vl(content.c_str(), content.length());
        vl.screenLineLen = 12;
        vl.screenLineCount = 20;//enough
        vl.wrapMode = 0;
        vl.maxLineLen = 1000;
        vl.lo->gotoFromBegin(0);
        auto vtest = vl.lines();
        for (int row = 0; row < vtest.lines->size(); row++) {
            wstring wstr = vtest[row];
            UTF utf;
            dstring dstr = utf.u16to32(wstr);
            int dlen = dstr.size();
            for (int col = 0; col < vl.screenLineLen; col++) {
                int64_t filePosition = vtest.filePosition(row, col);
                auto p = vtest.locatePosition(filePosition);
                if (col<dlen) {
                    EXPECT_EQ(p.first, row);
                    EXPECT_EQ(p.second, col);
                } else {
                    
                }
            }
        }
    }
}

TEST(locatePosition, fromEnd) {
    const string names[] = {"shortwrap.txt", "utf8.txt", "midlines.txt"};
    for (int k=0; k<3; k++) {
        string content = makeContent(string("../test/")+names[k]);
        ViewLogic vl(content.c_str(), content.length());
        vl.screenLineLen = 12;
        vl.screenLineCount = 3;
        vl.wrapMode = 0;
        vl.maxLineLen = 1000;
        vl.lo->gotoProportional(vl.fileSize);
        auto vtest = vl.lines();
        for (int row = 0; row < vtest.lines->size(); row++) {
            wstring wstr = vtest[row];
            UTF utf;
            dstring dstr = utf.u16to32(wstr);
            int dlen = dstr.size();
            for (int col = 0; col < vl.screenLineLen; col++) {
                int64_t filePosition = vtest.filePosition(row, col);
                auto p = vtest.locatePosition(filePosition);
                if (col<dlen) {
                    EXPECT_EQ(p.first, row);
                    EXPECT_EQ(p.second, col);
                }
            }
        }
    }
}

TEST(locatePosition, maxLine) {
    const string names[] = {"shortwrap.txt", "utf8.txt", "midlines.txt"};
    for (int k=0; k<3; k++) {
        string content = makeContent(string("../test/")+names[k]);
        ViewLogic vl(content.c_str(), content.length());
        vl.screenLineLen = 12;
        vl.screenLineCount = 12;
        vl.wrapMode = 0;
        vl.maxLineLen = 1000;
        vl.lo->gotoProportional(vl.fileSize);
        auto vtest = vl.lines();
        for (int row = 0; row < vtest.lines->size(); row++) {
            wstring wstr = vtest[row];
            UTF utf;
            dstring dstr = utf.u16to32(wstr);
            int dlen = dstr.size();
            for (int col = 0; col < vl.screenLineLen; col++) {
                int64_t filePosition = vtest.filePosition(row, col);
                auto p = vtest.locatePosition(filePosition);
                if (col<dlen) {
                    EXPECT_EQ(p.first, row);
                    EXPECT_EQ(p.second, col);
                }
            }
        }
    }
}

TEST(locatePosition, wrap1) {
    const string names[] = {"shortwrap.txt", "utf8.txt", "midlines.txt"};
    for (int k=0; k<3; k++) {
        string content = makeContent(string("../test/")+names[k]);
        ViewLogic vl(content.c_str(), content.length());
        vl.screenLineLen = 10;
        vl.screenLineCount = 16;
        vl.wrapMode = 1;
        vl.maxLineLen = 1000;
        vl.lo->gotoProportional(vl.fileSize);
        auto vtest = vl.lines();
        for (int row = 0; row < vtest.lines->size(); row++) {
            wstring wstr = vtest[row];
            UTF utf;
            dstring dstr = utf.u16to32(wstr);
            int dlen = dstr.size();
            for (int col = 0; col < vl.screenLineLen; col++) {
                int64_t filePosition = vtest.filePosition(row, col);
                auto p = vtest.locatePosition(filePosition);
                if (col<dlen) {
                    EXPECT_EQ(p.first, row);
                    EXPECT_EQ(p.second, col);
                }
            }
        }
    }
}


TEST(locatePosition, wrap2) {
    const string names[] = {"shortwrap.txt", "utf8.txt", "midlines.txt"};
    for (int k=0; k<3; k++) {
        string content = makeContent(string("../test/")+names[k]);
        ViewLogic vl(content.c_str(), content.length());
        vl.screenLineLen = 10;
        vl.screenLineCount = 16;
        vl.wrapMode = 2;
        vl.maxLineLen = 1000;
        vl.lo->gotoProportional(vl.fileSize);
        auto vtest = vl.lines();
        for (int row = 0; row < vtest.lines->size(); row++) {
            wstring wstr = vtest[row];
            UTF utf;
            dstring dstr = utf.u16to32(wstr);
            int dlen = dstr.size();
            for (int col = 0; col < vl.screenLineLen; col++) {
                int64_t filePosition = vtest.filePosition(row, col);
                auto p = vtest.locatePosition(filePosition);
                if (col<dlen) {
                    EXPECT_EQ(p.first, row);
                    EXPECT_EQ(p.second, col);
                }
            }
        }
    }
}

TEST(locatePosition, beginX) {
    const string names[] = {"shortwrap.txt", "utf8.txt", "midlines.txt"};
    for (int k=0; k<3; k++) {
        string content = makeContent(string("../test/")+names[k]);
        ViewLogic vl(content.c_str(), content.length());
        vl.screenLineLen = 10;
        vl.screenLineCount = 10;
        vl.wrapMode = 0;
        vl.maxLineLen = 1000;
        vl.lo->gotoProportional(vl.fileSize,2);
        auto vtest = vl.lines();
        for (int row = 0; row < vtest.lines->size(); row++) {
            wstring wstr = vtest[row];
            UTF utf;
            dstring dstr = utf.u16to32(wstr);
            int dlen = dstr.size();
            for (int col = 0; col < vl.screenLineLen; col++) {
                int64_t filePosition = vtest.filePosition(row, col);
                auto p = vtest.locatePosition(filePosition);
                if (col<dlen) {
                    EXPECT_EQ(p.first, row);
                    EXPECT_EQ(p.second, col);
                }
            }
        }
    }
}

TEST(locatePosition, before) {
    string content = makeContent("../test/midlines.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 5;
    vl.wrapMode = 0;
    vl.maxLineLen = 1000;
    vl.lo->gotoProportional(vl.fileSize);
    auto vtest = vl.lines();
    auto p = vtest.locatePosition(0);
    EXPECT_EQ(p.first,-1);
    EXPECT_EQ(p.second,0);
    int64_t begPos = vtest.filePosition(0,0);
    p = vtest.locatePosition(begPos-1);
    EXPECT_EQ(p.first,-1);
    EXPECT_EQ(p.second,0);
}

TEST(locatePosition, beforeWrap) {
    string content = makeContent("../test/midlines.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 5;
    vl.wrapMode = 1;
    vl.maxLineLen = 1000;
    vl.lo->gotoProportional(vl.fileSize);
    auto vtest = vl.lines();
    auto p = vtest.locatePosition(0);
    EXPECT_EQ(p.first,-1);
    EXPECT_EQ(p.second,0);
    int64_t begPos = vtest.filePosition(0,0);
    p = vtest.locatePosition(begPos-1);
    EXPECT_EQ(p.first,-1);
    EXPECT_EQ(p.second,0);
}

TEST(locatePosition, after) {
    string content = makeContent("../test/midlines.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 5;
    vl.wrapMode = 0;
    vl.maxLineLen = 1000;
    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    auto p = vtest.locatePosition(vl.fileSize);
    EXPECT_EQ(p.first,vl.screenLineCount);
    EXPECT_EQ(p.second,0);
}

TEST(locatePosition, afterWrap) {
    string content = makeContent("../test/midlines.txt");
    ViewLogic vl(content.c_str(), content.length());
    vl.screenLineLen = 10;
    vl.screenLineCount = 5;
    vl.wrapMode = 1;
    vl.maxLineLen = 1000;
    vl.lo->gotoFromBegin(0);
    auto vtest = vl.lines();
    auto p = vtest.locatePosition(vl.fileSize);
    EXPECT_EQ(p.first,vl.screenLineCount);
    EXPECT_EQ(p.second,0);
}