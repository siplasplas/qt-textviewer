//
// Created by Andrzej Borucki on 2022-09-11
//

#ifndef VIEWER_VIEWLOGIC_H
#define VIEWER_VIEWLOGIC_H

#include "Lines.h"

namespace vl {
    struct ViewResult {
        InfoVec* infos = nullptr;
        LineVec* lines = nullptr;
        size_t size() {
            return lines->size();
        }
        std::wstring operator[](size_t index) {
            return lines->at(index).text;
        }
    };

    class ViewLogic {
        const char *addr;
        int64_t fileSize;
        int BOMsize = 0;
        friend class LineOwner;
    public:
        ViewLogic(const char *addr, int64_t fileSize) : addr(addr), fileSize(fileSize) {}
        int screenLineLen = 0;
        int screenLineCount = 0;
        ViewResult linesFromBeginScreen(int64_t start);
    private:
        ViewResult linesFromBeginScreen(LineOwner start);
        ViewResult infosFromBeginScreen(int64_t start);
        void fillLines(ViewResult &vr);
        void updateInfo(int64_t offset, LineInfo* li);
    };

} // vl

#endif //VIEWER_VIEWLOGIC_H
