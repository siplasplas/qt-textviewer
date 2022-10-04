//
// Created by andrzej on 10/4/22.
//

#include "Selection.h"

bool Selection::charSelected(QPoint point, vl::ViewResult &vr) {
    int64_t pos = vr.filePosition(point.y(), point.x());
    return pos>=selBegin && pos<selEnd;
}
