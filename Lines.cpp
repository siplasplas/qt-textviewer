//
// Created by Andrzej Borucki on 2022-09-11
//
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

    LineOwner::LineOwner(int64_t offset, bool wrap, ViewLogic* vl) : vl(vl) {
        int64_t linePos = vl->gotoBeginLine(offset);
        li = new LineInfo;
        vl->updateInfo(linePos, li);
    }

    void LineOwner::clone(const LineOwner &src) {
        if (li)
            delete li;
        li = new LineInfo;
        *li = *src.li;
        vl = src.vl;
    }

    LineOwner::LineOwner(const LineOwner &src) {
        clone(src);
    }

    LineOwner &LineOwner::operator=( const LineOwner &src) {
        if(this == &src){
            return *this;
        }
        clone(src);
        return *this;
    }

    void LineOwner::backNlines(int n) {
        for (int i=0; i<n; i++)
            backLine();
    }

    void LineOwner::backLine() {
        int64_t linePos = vl->gotoBeginLine(li->offset-1);
        vl->updateInfo(linePos, li);
    }

}