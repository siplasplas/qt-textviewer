//
// Created by Andrzej Borucki on 2022-09-11
//

#ifndef VIEWER_LINES_H
#define VIEWER_LINES_H

#include <cstdint>
#include <string>
#include <vector>

namespace vl {

    struct LineInfo {
        int64_t offset;
        int len; //without endline char, can be zero
        int64_t next;
        std::vector<int> wrapLens;
        int64_t wrapOffset(int wrapIndex);
        bool operator==(const LineInfo &src) const;
        bool operator!=(const LineInfo &src) const;
    };

    struct Line {
        std::wstring text;
        const LineInfo *li = nullptr;
        int wrapIndex = -1;
        Line(std::wstring text, const LineInfo *li, int wrapIndex):
                text(std::move(text)), li(li), wrapIndex(wrapIndex) {}
        bool operator==(const Line &src) const;
    };

    using InfoVec = std::vector<LineInfo *>;
    using LineVec = std::vector<Line>;

    class ViewLogic;

    class LineOwner {
        ViewLogic *vl = nullptr;
        int64_t position = 0;
        bool wrap;
        int backCount = 0;
    public:
        explicit LineOwner(ViewLogic *vl);
        int beginX = 0;
        LineInfo *li = nullptr;
        int wrapIndex = -1;
        void gotoProportional(int64_t position, int beginX);
        void gotoFromBegin(int64_t position, int beginX);
        void initLi(int64_t offset, bool wrap);
        ~LineOwner() {
            delete li;
        }
        void update();
        void backNlines(int n);
        void backLine();
        int getWrapIndex(int64_t offset);
    };

}
#endif //VIEWER_LINES_H
