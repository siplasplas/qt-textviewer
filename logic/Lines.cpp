//
// Created by Andrzej Borucki on 2022-09-11
//
#include <cassert>
#include <cmath>
#include "Lines.h"
#include "ViewLogic.h"

namespace vl {

    bool Line::operator==(const Line &src) const {
        if (text!=src.text) return false;
        if (*li!=*src.li) return false;
        return true;
    }

    bool LineInfo::operator==(const LineInfo &src) const {
        if (offset!=src.offset) return false;
        if (len!=src.len) return false;
        if (next!=src.next) return false;
        return true;
    }

    bool LineInfo::operator!=(const LineInfo &src) const {
        return !(*this==src);
    }

    int64_t LineInfo::wrapOffset(int wrapIndex) const {
        int64_t woffset = offset;
        for (int i=0; i<wrapIndex; i++)
            woffset += wrapLens[i];
        return woffset;
    }

    int64_t LineInfo::wrapOffsetEnd(int wrapIndex) const {
        int64_t woffset = offset;
        for (int i=0; i<=wrapIndex; i++)
            woffset += wrapLens[i];
        return woffset;
    }

    LineOwner::LineOwner(ViewLogic* vl) : vl(vl) {}

    void LineOwner::initLi(int64_t offset, bool wrap) {
        this->wrap = wrap;
        int64_t linePos = vl->gotoBeginLine(offset);
        delete li;
        li = new LineInfo;
        vl->updateInfo(linePos, li);
        if (wrap) {
            wrapIndex = getWrapIndex(offset);
            //if after loop wrapIndex == li->wrapLens.size() => position after line, at LF char
        }
    }

    void LineOwner::backNlines(int n, int64_t linePos) {
        for (int i=0; i<n; i++) {
            backLine(linePos);
            if (linePos<=vl->BOMsize) break;
        }
    }

    void LineOwner::backLine(int64_t &linePos) {
        if (wrap) {
            wrapIndex--;
            if (wrapIndex<0) {
                linePos = vl->gotoBeginLine(li->offset-1);
                vl->updateInfo(linePos, li);
                assert(!li->wrapLens.empty() || !li->len);
                wrapIndex = (int)li->wrapLens.size()-1;
            }
        }
        else {
            linePos = vl->gotoBeginLine(li->offset-1);
            vl->updateInfo(linePos, li);
        }
    }

    int LineOwner::getWrapIndex(int64_t offset) {
        int64_t wrapPosition = li->offset + li->len;
        int resultIndex = 0;
        for (int i=0; i < li->wrapLens.size(); ++i) {
            int len = li->wrapLens[i];
            wrapPosition += len;
            if(wrapPosition>offset) {
                resultIndex = i;
                break;
            }
        }
        return resultIndex;
    }

    void LineOwner::update() {
        initLi(position, vl->wrapMode > 0);
        backNlines(backCount,position);
    }

    void LineOwner::gotoFromBegin(int64_t position, int beginX) {
        this->position = std::min(position, vl->fileSize);
        this->beginX = beginX;
        backCount = 0;
        update();
    }

    void LineOwner::gotoProportional(int64_t position, int beginX) {
        this->beginX = beginX;
        assert(vl->maxLineLen>=UTF::MAXCHARLEN);
        assert(vl->fileSize>=0);
        this->position = std::min(position, vl->fileSize);
        assert(vl->screenLineCount>=0);
        assert(this->position<=vl->fileSize);
        if (vl->fileSize>0 && vl->screenLineCount>0) {
            if (position < vl->fileSize) {
                backCount = ceill((long double) (position - vl->BOMsize) / (vl->fileSize - vl->BOMsize) *
                                  (vl->screenLineCount - 1));
            } else
                backCount = ceill(
                        (long double) (position - vl->BOMsize) / (vl->fileSize - vl->BOMsize) * vl->screenLineCount);
        } else backCount = 0;
        update();
    }

}