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
        bool operator==( const ViewResult &src) const;
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
        LineOwner getBeginPos(int64_t position);
        ViewResult lines(int64_t position);
    private:
        ViewResult linesFromBeginScreen(const LineOwner& start);
        ViewResult infosFromBeginScreen(int64_t start);
        void fillLines(ViewResult &vr);
        void updateInfo(int64_t offset, LineInfo* li);
        bool isNewlineChar(const char c);
        int64_t searchEndOfLine(int64_t startOffset);
        int64_t skipLineBreak(int64_t pos);
        std::wstring fillWithScreenLen(int64_t offset, int len);
        int64_t gotoBeginLine(int64_t offset);
        int64_t gotoFirstOfCRLF(int64_t offset);
        bool lineIsEmpty(int64_t offset);
        int64_t gotoBeginNonEmptyLine(int64_t start);
    };

} // vl

#endif //VIEWER_VIEWLOGIC_H
