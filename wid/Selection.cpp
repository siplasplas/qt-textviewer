//
// Created by andrzej on 10/4/22.
//

#include "Selection.h"

bool Selection::charSelected(QPoint point, vl::ViewResult &vr) {
    int64_t pos = vr.filePosition(point.y(), point.x());
    return pos>=selBegin && pos<selEnd;
}

void Selection::compute(vl::ViewResult &vr) {
    selBeginScreen = vr.locatePosition(selBegin);
    selEndScreen = vr.locatePosition(selEnd);
}

int Selection::selColBeg(int row, vl::ViewResult &vr) {
    if (row>=selBeginScreen.first && row<=selEndScreen.first) {
        if (row==selBeginScreen.first)
            return selBeginScreen.second;
        else
            return 0;
    }
    else
        return -1;
}

int Selection::selColEnd(int row, vl::ViewResult &vr) {
    if (row>=selBeginScreen.first && row<=selEndScreen.first) {
        if (row==selEndScreen.first)
            return selEndScreen.second;
        else
            return -1;
    }
    else
        return -1;
}
