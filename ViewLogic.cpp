//
// Created by Andrzej Borucki on 2022-09-11
//

#include <cassert>
#include <cmath>
#include "ViewLogic.h"

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
        return linesFromBeginScreen(LineOwner(start, wrapMode>0, this));
    }

    ViewResult ViewLogic::linesFromBeginScreen(const LineOwner& start) {
        auto vr= infosFromBeginScreen(start.li->offset);
        vr.firstWrapIndex = start.wrapIndex;
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
        if (wrapMode>0)
            li->wrapLens = computeWrapLens(offset, li->len);
        else
            li->wrapLens.clear();
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
        if (offset == possibleBreakAt && isFirstChunkStart(startOffset)) {
            possibleBreakAt += maxLineLen;
            while(offset<fileSize && !isNewlineChar(addr[offset]) && offset < possibleBreakAt)
                offset++;
        }
        assert(offset==fileSize || isNewlineChar(addr[offset])|| offset==possibleBreakAt);
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
        for (int k=0; k<vr.infos->size(); k++) {
            auto li = vr.infos->at(k);
            assert(li->len>=0);
            if (!li->len)
                vr.lines->emplace_back(Line(L"",li, -1));
            else if (wrapMode==0){
                wstring wstr = fillWithScreenLen(li->offset, li->len);
                vr.lines->emplace_back(Line(wstr, li, -1));
            } else {
                assert(!li->wrapLens.empty());
                int64_t wrapOffset = li->offset;
                assert(vr.firstWrapIndex>=0);
                int start = k==0? vr.firstWrapIndex:0;
                for (int i=0; i<start; i++)
                    wrapOffset += li->wrapLens[i];
                for (int i=start; i<li->wrapLens.size(); i++)
                {
                    int wrapLen = li->wrapLens[i];
                    if (vr.lines->size()>=screenLineCount) break;
                    wstring wstr = fillWithScreenLen(wrapOffset, wrapLen);
                    vr.lines->emplace_back(Line(wstr, li, i));
                    wrapOffset += wrapLen;
                }
            }
        }
    }

    wstring ViewLogic::fillWithScreenLen(int64_t offset, int len) {
        UTF utf;
        dstring dstr(screenLineLen);
        const char *s = addr + offset;
        const char *eol = addr + offset + len;
        int k=0;
        while (s<eol && k<screenLineLen) {
            const char *end;
            dstr[k] = utf.one8to32(s, eol, &end);
            s = end;
            k++;
        }
        dstr.resize(k);
        return utf.u32to16(dstr);
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
        while (offset>BOMsize && !isNewlineChar(addr[offset-1]) ) {
            if (offset==possibleBreakAt) {
                if (!isFirstChunkInside(offset))
                    return offset;
            }
            offset--;
        }
        return offset;
    }

    LineOwner ViewLogic::getBeginPos(int64_t position) {
        assert(fileSize>=0);
        if (!fileSize) return {};
        position = min(position, fileSize);
        assert(screenLineCount>=0);
        assert(position<=fileSize);
        if (!screenLineCount) return {position, wrapMode>0,this};
        int backCount;
        if (position<fileSize) {
            backCount = ceill((long double)(position-BOMsize) / (fileSize-BOMsize) * (screenLineCount-1));
        } else
            backCount = ceill((long double)(position-BOMsize) / (fileSize-BOMsize) * screenLineCount);
        LineOwner lineOwner(position,wrapMode>0, this);
        lineOwner.backNlines(backCount);
        return lineOwner;
    }

    ViewResult ViewLogic::lines(int64_t position) {
        return linesFromBeginScreen(getBeginPos(position));
    }

    bool ViewLogic::isFirstChunkStart(int64_t offset) {
        return offset<=BOMsize || isNewlineChar(addr[offset-1]);
    }

    bool ViewLogic::isFirstChunkInside(int64_t offset) {
        return !startInsideSegment(offset-maxLineLen);
    }

    bool ViewLogic::startInsideSegment(int64_t offset) {
        if (offset<=BOMsize) return false;
        int64_t nSegment = offset/maxLineLen;
        int64_t start = (max((int64_t)BOMsize+1, nSegment*maxLineLen))-1;
        int64_t end = (nSegment*maxLineLen+maxLineLen)-1;
        for (int64_t pos = start; pos<end; pos++)
            if (isNewlineChar(addr[pos])) {
                return false;
            }
        return true;
    }

    vector<int> ViewLogic::computeWrapLens(int64_t offset, int len) {
        vector<int> screenLines;
        const char *s = addr + offset;
        const char *seol = s + len;
        const char *sprev = s;
        dstring dstr(screenLineLen); //need buffer for wordWrap
        int width = 0;
        while (s < seol) {
            UTF utf;
            const char *end;
            dstr[width] = utf.one8to32(s, seol, &end);
            s = end;
            width++;
            if (width == screenLineLen) {
                //------
                if (wrapMode == 2) {
                    uint cl = codeClass(dstr[width - 1]);
                    if (cl > 0) {
                        int countLast = clLastWidth(dstr, width, cl);
                        int countNext = clNextWidth(s, seol, cl);
                        if (countNext>0 && countLast+countNext<=screenLineLen) {
                            s = utf.backNcodes(countLast, s, sprev);
                            width -= countLast;
                        }
                    }
                }
                //------
                screenLines.push_back(int(s - sprev));
                width = 0;
                sprev = s;
            }
        }
        //last, shorter line
        if (width > 0 || len==0) {
            screenLines.push_back(int(s - sprev));
        }
        assert(!screenLines.empty());
        return screenLines;
    }

    uint ViewLogic::codeClass(unsigned int c) {
        if (isNewlineChar(char(c)))
            return -1;
        if (c==' ' || c=='\t')
            return 0;
        else if (c=='_' || c>='A' && c<='Z' || c>='a' && c<='z' || c>='0' && c<='9'
                 || c>=0x80 && c<0x250)
            return 1;
        else if (c<128)
            return 2;
        else
            return c & 0xffffff00;
    }

    int ViewLogic::clLastWidth(const dstring &dstr, int width, uint cl) {
        int countLast = 0;
        for (int k=width-1; k>=0; k--) {
            if (codeClass(dstr[k]) == cl)
                countLast++;
            else
                break;
        }
        return countLast;
    }

    int ViewLogic::clNextWidth(const char *s, const char *seol, uint cl) {
        int countNext= 0;
        const char *s1 = s;
        UTF utf;
        while (s1<seol && countNext < screenLineLen) {
            const char *end;
            uint d = utf.one8to32(s1, seol, &end);//no need here keep an eye on \t
            s1 = end;
            if (codeClass(d) != cl)
                break;
            countNext++;
            s1++;
        }
        return countNext;
    }

} // vl