//
// Created by Andrzej Borucki on 2022-09-11
//
#include "Lines.h"
#include "ViewLogic.h"

namespace vl {

    LineOwner::LineOwner(int64_t offset, ViewLogic *vl) : vl(vl) {
        int64_t linePos = vl->gotoBeginLine(offset);
        li = new LineInfo;
        vl->updateInfo(linePos, li);
    }

}