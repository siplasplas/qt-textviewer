//
// Created by Andrzej Borucki on 2022-09-11
//

#include <cassert>
#include <cmath>
#include "ViewLogic.h"

using namespace std;

namespace vl {
    ViewResult::ViewResult(const ViewResult &src) {
        clone(src);
    }

    ViewResult &ViewResult::operator=( const ViewResult &src) {
        if(this == &src){
            return *this;
        }
        clone(src);
        return *this;
    }

    void ViewResult::clone( const ViewResult &src) {
        wrap = src.wrap;
        firstWrapIndex = src.firstWrapIndex;
        lastWrapIndex = src.lastWrapIndex;
        if (src.infos) {
            if (!infos)
                infos = new InfoVec;
            *infos = *src.infos;
        } else {
            delete infos;
            infos = nullptr;
        }
        if (src.lines) {
            if (!lines)
                lines = new LineVec;
            *lines = *src.lines;
        } else {
            delete lines;
            lines = nullptr;
        }
    }

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
        int64_t possibleBreakCorrected = correctPossibleBreak(possibleBreakAt);
        while ( offset<fileSize && !isNewlineChar(addr[offset]) && offset < possibleBreakCorrected )
            offset++;
        assert(offset==fileSize || isNewlineChar(addr[offset]) || offset==possibleBreakCorrected );
        if (offset == possibleBreakCorrected && isFirstChunkStart(startOffset)) {
            possibleBreakAt += maxLineLen;
            possibleBreakCorrected = correctPossibleBreak(possibleBreakAt);
            while(offset<fileSize && !isNewlineChar(addr[offset]) && offset < possibleBreakCorrected)
                offset++;
        }
        assert(offset==fileSize || isNewlineChar(addr[offset])|| offset==possibleBreakCorrected);
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

    Line ViewLogic::fillLine(LineInfo *li, int wrapIndex) {
        if (!li->len)
            return Line(L"",li, -1);
        if (wrapIndex<0) {
            wstring wstr = fillWithScreenLen(li->offset, li->len);
            return Line(wstr, li, -1);
        } else {
            wstring wstr = fillWithScreenLen(li->wrapOffset(wrapIndex), li->wrapLens[wrapIndex]);
            return Line(wstr, li, wrapIndex);
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
            if (*s=='\t') {
                s++;
                dstr[k] = ' '; //minimal space width==1
                k++;
                while (k<screenLineLen && (k % maxTabW) != 0) {
                    dstr[k] = ' ';
                    k++;
                }
            } else {
                const char *end;
                dstr[k] = utf.one8to32(s, eol, &end);
                s = end;
                k++;
            }
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
        int64_t possibleBreakCorrected = correctPossibleBreak(possibleBreakAt);
        if (possibleBreakCorrected>start && possibleBreakAt>=maxLineLen) {
            possibleBreakAt-=maxLineLen;
            possibleBreakCorrected = correctPossibleBreak(possibleBreakAt);
        }
        int64_t offset = start;
        while (offset>BOMsize && !isNewlineChar(addr[offset-1])) {
            if (offset==possibleBreakCorrected) {
                if (!isFirstChunkInside(offset))
                    return offset;
            }
            offset--;
        }
        return offset;
    }

    LineOwner ViewLogic::getBeginPos(int64_t position) {
        assert(maxLineLen>=UTF::MAXCHARLEN);
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
        if (offset<=BOMsize+UTF::MAXCHARLEN-1) return false;
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
            if (*s=='\t') {
                s++;
                dstr[width] = ' '; //minimal space width==1
                width++;
                while (width<screenLineLen && (width % maxTabW) != 0) {
                    dstr[width] = ' ';
                    width++;
                }
            }
            else
            {
                const char *end;
                dstr[width] = utf.one8to32(s, seol, &end);
                s = end;
                width++;
            }
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

    int64_t ViewLogic::correctPossibleBreak(int64_t possibleBreakAt) {
        if (possibleBreakAt<BOMsize || possibleBreakAt>=fileSize)
            return possibleBreakAt;
        if (addr[possibleBreakAt]=='\n' && possibleBreakAt>BOMsize && addr[possibleBreakAt-1]=='\r')
            return possibleBreakAt+1;
        UTF utf;
        return utf.findNextUtf8OrTheSame(addr + possibleBreakAt, addr + std::max((int64_t)BOMsize, possibleBreakAt - (UTF::MAXCHARLEN-1)), addr + fileSize) - addr;
    }

    //return actually scrolls done
    int ViewLogic::scrollDown(ViewResult &vr) {
        if (vr.infos->empty()) return 0;
        auto *lastLi = vr.infos->back();
        assert(lastLi->next<=fileSize);
        if (vr.wrap) {
            assert(vr.lastWrapIndex >= 0 && vr.lastWrapIndex < lastLi->wrapLens.size());
            if (lastLi->next==fileSize && vr.lastWrapIndex == lastLi->wrapLens.size()) return 0;
            vr.lastWrapIndex++;
            if (vr.lastWrapIndex == lastLi->wrapLens.size()) {
                vr.lastWrapIndex = 0;
                auto *li = new LineInfo;
                updateInfo(lastLi->next, li);
                vr.infos->push_back(li);
                vr.infos->erase(vr.infos->begin());
            }
        } else {
            if (lastLi->next==fileSize) return 0;
            auto *li = new LineInfo;
            updateInfo(lastLi->next, li);
            vr.infos->push_back(li);
            vr.infos->erase(vr.infos->begin());
        }
        auto newLi = vr.infos->back();
        auto newLine = fillLine(newLi, vr.lastWrapIndex);
        vr.lines->push_back(newLine);
        vr.lines->erase(vr.lines->begin());
        return 1;
    }

    int ViewLogic::scrollUp(ViewResult &vr) {
        if (vr.infos->empty()) return 0;
        auto *firstLi = vr.infos->at(0);
        assert(firstLi->offset>=BOMsize);
        if (vr.wrap) {
            assert(vr.firstWrapIndex >= 0);
            if (firstLi->offset==BOMsize && vr.firstWrapIndex==0) return 0;
            vr.firstWrapIndex--;
            if (vr.firstWrapIndex < 0) {
                vr.firstWrapIndex = firstLi->wrapLens.size() - 1;
                auto *li = new LineInfo;
                int64_t linePos = gotoBeginLine(li->offset-1);
                updateInfo(linePos, li);
                vr.infos->insert(vr.infos->begin(), li);
                vr.infos->pop_back();
            }
        } else {
            auto *li = new LineInfo;
            if (firstLi->offset==BOMsize) return 0;
            int64_t linePos = gotoBeginLine(firstLi->offset-1);
            updateInfo(linePos, li);
            vr.infos->insert(vr.infos->begin(), li);
            vr.infos->pop_back();
        }
        auto newLi = vr.infos->at(0);
        auto newLine = fillLine(newLi, vr.firstWrapIndex);
        vr.lines->insert(vr.lines->begin(), newLine);
        vr.lines->pop_back();
        return 1;
    }

    bool ViewLogic::scrollNDown(int n, ViewResult &vr) {
        if (vr.infos->empty()) return false;
        int result = 0;
        for (int i=0; i<n; i++) {
            int res = scrollDown(vr);
            if (res<1)
                return result;
            result += res;
        }
        return result;
    }

    bool ViewLogic::scrollNUp(int n, ViewResult &vr) {
        if (vr.infos->empty()) return false;
        int result = 0;
        for (int i=0; i<n; i++) {
            int res = scrollUp(vr);
            if (res<1)
                return result;
            result += res;
        }
        return result;
    }

    bool ViewLogic::scrollPageDown(ViewResult &vr) {
        return scrollNDown(screenLineCount, vr);
    }

    bool ViewLogic::scrollPageUp(ViewResult &vr) {
        return scrollNUp(screenLineCount, vr);
    }

} // vl