//
// Created by Andrzej Borucki on 2022-09-11
//

#ifndef VIEWER_VIEWLOGIC_H
#define VIEWER_VIEWLOGIC_H

#include "Lines.h"
#include "../utf/UTF.hpp"
#include "Cache.h"
#include "CacheX.h"

namespace vl {

    struct ViewResult {
        InfoVec* infos = nullptr;
        LineVec* lines = nullptr;
        int firstWrapIndex = -1;
        int lastWrapIndex = -1;
        int beginX = 0;
        bool wrap;
        ViewLogic *vl;
        size_t size() {
            return lines->size();
        }
        std::wstring operator[](size_t index) {
            return lines->at(index).text;
        }
        ViewResult() = default;
        ~ViewResult() {
            delete infos;
            delete lines;
            delete start;
        }
        ViewResult( const ViewResult& );
        ViewResult &operator=( const ViewResult &src);
        bool operator==( const ViewResult &src) const;
        void clone(const ViewResult &src);
        const LineOwner *start = nullptr;
        int64_t filePosition(int row, int col);
    };

    class ViewLogic {
        friend class LineOwner;
    public:
        ViewLogic(const char *addr, int64_t fileSize) : addr(addr), fileSize(fileSize) {
            lo = new LineOwner(this);
        }
        ~ViewLogic() {delete lo;}
        const char * const addr;
        const int64_t fileSize;
        const int BOMsize = 0;
        int screenLineLen = 0;
        int screenLineCount = 0;
        int maxLineLen = 1000000;
        int wrapMode = 0;
        int maxTabW = 1;
        int minLineToCacheX = 200;
        LineOwner* lo = nullptr;
        ViewResult lines();
        int scrollDown(ViewResult &vr);
        int scrollUp(ViewResult &vr);
        bool scrollNDown(int n, ViewResult &vr);
        bool scrollNUp(int n, ViewResult &vr);
        bool scrollPageDown(ViewResult &vr);
        bool scrollPageUp(ViewResult &vr);
    private:
        Cache cache;
        CacheX cacheX;
        const int minLineToCache = 1000;
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
        bool isFirstChunkStart(int64_t offset);
        bool isFirstChunkInside(int64_t offset);
        bool startInsideSegment(int64_t offset);
        std::vector<int> computeWrapLens(int64_t offset, int len);
        uint codeClass(unsigned int c);
        int clLastWidth(const dstring &dstr, int width, uint cl);
        int clNextWidth(const char *s, const char *seol, uint cl);
        int64_t correctPossibleBreak(int64_t possibleBreakAt);
        Line fillLine(LineInfo *li, int wrapIndex);
        void fillLines_loop(ViewResult &vr);
        int computeRawBeginX(int64_t startLine, int lineLen, int beginX);
    };

} // vl

#endif //VIEWER_VIEWLOGIC_H
