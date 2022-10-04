//
// Created by Andrzej Borucki on 2022-10-04
//
#include "ViewLogic.h"
#include "Util.h"
#include "gtest/gtest.h"

using namespace std;
using namespace vl;

TEST(filePosition, whole) {
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
                if (row == vtest.lines->size() - 1 && col >= dlen) {
                    EXPECT_EQ(filePosition, vl.fileSize);
                    continue;
                }
                const char *send;
                uint32_t expectChar = utf.one8to32(vl.addr+filePosition, vl.addr+vl.fileSize, &send);
                if (col < dlen) {
                    EXPECT_EQ(dstr[col], expectChar);
                } else {
                    EXPECT_EQ(expectChar==10 || expectChar==13, true);
                }
            }
        }
    }
}

TEST(filePosition, fromEnd) {
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
                if (row == vtest.lines->size() - 1 && col >= dlen) {
                    EXPECT_EQ(filePosition, vl.fileSize);
                    continue;
                }
                const char *send;
                uint32_t expectChar = utf.one8to32(vl.addr+filePosition, vl.addr+vl.fileSize, &send);
                if (col < dlen) {
                    EXPECT_EQ(dstr[col], expectChar);
                } else {
                    EXPECT_EQ(expectChar==10 || expectChar==13, true);
                }
            }
        }
    }
}

TEST(filePosition, maxLine) {
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
                if (row == vtest.lines->size() - 1 && col >= dlen) {
                    EXPECT_EQ(filePosition, vl.fileSize);
                    continue;
                }
                const char *send;
                uint32_t expectChar = utf.one8to32(vl.addr+filePosition, vl.addr+vl.fileSize, &send);
                if (col < dlen) {
                    EXPECT_EQ(dstr[col], expectChar);
                } else {
                    EXPECT_EQ(expectChar==10 || expectChar==13, true);
                }
            }
        }
    }
}

TEST(filePosition, wrap1) {
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
                if (row == vtest.lines->size() - 1 && col >= dlen) {
                    EXPECT_EQ(filePosition, vl.fileSize);
                    continue;
                }
                const char *send;
                uint32_t expectChar = utf.one8to32(vl.addr+filePosition, vl.addr+vl.fileSize, &send);
                if (col < dlen) {
                    EXPECT_EQ(dstr[col], expectChar);
                } else {
                    EXPECT_EQ(expectChar==10 || expectChar==13, true);
                }
            }
        }
    }
}


TEST(filePosition, wrap2) {
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
                if (row == vtest.lines->size() - 1 && col >= dlen) {
                    EXPECT_EQ(filePosition, vl.fileSize);
                    continue;
                }
                const char *send;
                uint32_t expectChar = utf.one8to32(vl.addr+filePosition, vl.addr+vl.fileSize, &send);
                if (col < dlen) {
                    EXPECT_EQ(dstr[col], expectChar);
                } else {
                    int64_t prevFilePosition = vtest.filePosition(row, dlen-1);
                    if (dlen>0)
                        EXPECT_EQ(filePosition, prevFilePosition+utf.one8len(dstr[dlen-1]));
                }
            }
        }
    }
}

TEST(filePosition, beginX) {
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
                if (row == vtest.lines->size() - 1 && col >= dlen) {
                    EXPECT_EQ(filePosition, vl.fileSize);
                    continue;
                }
                const char *send;
                uint32_t expectChar = utf.one8to32(vl.addr+filePosition, vl.addr+vl.fileSize, &send);
                if (col < dlen) {
                    EXPECT_EQ(dstr[col], expectChar);
                } else {
                    EXPECT_EQ(expectChar==10 || expectChar==13, true);
                }
            }
        }
    }
}
