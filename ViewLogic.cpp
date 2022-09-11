//
// Created by Andrzej Borucki on 2022-09-11
//

#include <cassert>
#include <cmath>
#include "ViewLogic.h"
#include "utf/UTF.hpp"

using namespace std;

namespace vl {

    bool ViewResult::operator==(const ViewResult &src) const {
        if (infos && !src.infos) return false;
        if (!infos && src.infos) return false;
        if (infos->size() != src.infos->size()) return false;
        for(int i=0; i<infos->size(); i++)
            if (*infos->at(i) != *src.infos->at(i)) return false;
        if (lines && !src.lines) return false;
        if (!lines && src.lines) return false;
        if (lines && *lines!=*src.lines) return false;
        return true;
    }

    ViewResult ViewLogic::infosFromBeginScreen(int64_t start) {
        ViewResult vr;
        vr.infos = new InfoVec;
        int64_t pos = start;
        while (pos<fileSize && vr.infos->size()<screenLineCount) {
            auto *li = new LineInfo;
            updateInfo(pos, li);
            pos = li->next;
            vr.infos->push_back(li);
        }
        return vr;
    }

    ViewResult ViewLogic::linesFromBeginScreen(int64_t start) {
        return linesFromBeginScreen(LineOwner(start, this));
    }

    ViewResult ViewLogic::linesFromBeginScreen(const LineOwner& start) {
        auto vr= infosFromBeginScreen(start.li->offset);
        fillLines(vr);
        return vr;
    }

    void ViewLogic::updateInfo(int64_t offset, LineInfo *li) {
        li->offset = offset;
        int64_t endPos;
        if (isNewlineChar(addr[offset]))
            endPos = offset;
        else
            endPos = searchEndOfLine(offset);
        li->len = endPos-offset;
        if (li->len==0 && offset==fileSize-1)
            li->next = fileSize;
        else
            li->next = skipLineBreak(endPos);
    }

    bool ViewLogic::isNewlineChar(const char c) {
        return c=='\r' || c=='\n';
    }

    int64_t ViewLogic::searchEndOfLine(int64_t startOffset) {
        int64_t offset = startOffset;
        int64_t possibleBreakAt = (offset/maxLineLen)*maxLineLen+maxLineLen;
        while ( offset<fileSize && !isNewlineChar(addr[offset]) && offset < possibleBreakAt )
            offset++;
        assert(offset==fileSize || isNewlineChar(addr[offset]) || offset==possibleBreakAt );
        return offset;
    }

    //not skip linebreak at end of file
    int64_t ViewLogic::skipLineBreak(int64_t pos) {
        assert(pos<=fileSize);
        if (pos==fileSize) return pos;
        const char c = addr[pos];
        assert(pos<=fileSize-1);
        if (c=='\r' && pos+1<=fileSize-1 && addr[pos+1]=='\n')
            return min(pos+2,fileSize-1);
        else if (isNewlineChar(c))
            return min(pos+1,fileSize-1);
        else {//MaxLine break
            return pos;
        }
    }

    void ViewLogic::fillLines(ViewResult &vr) {
        delete vr.lines;
        vr.lines = new LineVec;
        for (auto *li: *vr.infos) {
            assert(li->len>=0);
            if (!li->len)
                vr.lines->emplace_back(Line(L"",li));
            else {
                wstring wstr = fillWithScreenLen(li->offset, li->len);
                vr.lines->emplace_back(Line(wstr, li));
            }
        }
    }

    wstring ViewLogic::fillWithScreenLen(int64_t offset, int len) {
        UTF utf;
        string str(addr+offset, len);
        wstring wstr = utf.u8to16(str);
        return wstr.substr(0,screenLineLen);
    }

    int64_t ViewLogic::gotoBeginLine(int64_t offset) {
        assert(offset<=fileSize);
        if (offset==fileSize) return fileSize;
        if (isNewlineChar(addr[offset])) {
            offset = gotoFirstOfCRLF(offset);
            if (lineIsEmpty(offset))
                return offset;
            offset--;
        }
        return gotoBeginNonEmptyLine(offset);
    }

    int64_t ViewLogic::gotoFirstOfCRLF(int64_t offset) {
        assert(offset<fileSize && isNewlineChar(addr[offset]));
        char c = addr[offset];
        if (c=='\r')
            return offset;
        else {
            if (offset<=BOMsize)
                return offset;
            else {
                char b = addr[offset-1];
                if (b=='\r')
                    return offset-1;
                else
                    return offset;
            }
        }
    }

    //last \n in file -> line is empty
    bool ViewLogic::lineIsEmpty(int64_t offset) {
        assert(offset<fileSize && isNewlineChar(addr[offset]));
        if (offset==fileSize-1)
            return true;
        if (offset==fileSize-2 && addr[offset]=='\r' && addr[offset+1]=='\n')
            return true;
        return offset<=BOMsize || isNewlineChar(addr[offset-1]);
    }

    int64_t ViewLogic::gotoBeginNonEmptyLine(int64_t start) {
        assert(start<fileSize && !isNewlineChar(addr[start]));
        int64_t possibleBreakAt = (start/maxLineLen)*maxLineLen;
        if (possibleBreakAt>=start)
            possibleBreakAt -= maxLineLen;
        int64_t offset = start;
        while (offset>BOMsize && !isNewlineChar(addr[offset-1]) && offset>possibleBreakAt)
            offset--;
        return offset;
    }

    LineOwner ViewLogic::getBeginPos(int64_t position) {
        assert(fileSize>=0);
        if (!fileSize) return {};
        position = min(position, fileSize);
        assert(screenLineCount>=0);
        assert(position<=fileSize);
        if (!screenLineCount) return {position, this};
        int backCount;
        if (position<fileSize) {
            backCount = ceill((long double)(position-BOMsize) / (fileSize-BOMsize) * (screenLineCount-1));
        } else
            backCount = ceill((long double)(position-BOMsize) / (fileSize-BOMsize) * screenLineCount);
        LineOwner lineOwner(position, this);
        lineOwner.backNlines(backCount);
        return lineOwner;
    }

    ViewResult ViewLogic::lines(int64_t position) {
        return linesFromBeginScreen(getBeginPos(position));
    }

} // vl