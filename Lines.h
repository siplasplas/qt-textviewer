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
    };

    struct Line {
        std::wstring text;
        const LineInfo *li = nullptr;

        Line(std::wstring text, const LineInfo *li) :
                text(std::move(text)), li(li) {}
    };

    using InfoVec = std::vector<LineInfo *>;
    using LineVec = std::vector<Line>;

    class ViewLogic;

    struct LineOwner {
        LineInfo *li = nullptr;
        ViewLogic *vl;
        LineOwner() = default;
        LineOwner(int64_t offset, ViewLogic *vl);
        ~LineOwner() {
            delete li;
        }
        void backNlines(int n);
        void backLine();
        LineOwner(const LineOwner &src);
        LineOwner &operator=( const LineOwner &src);
        void clone(const LineOwner &src);
    };

}
#endif //VIEWER_LINES_H
