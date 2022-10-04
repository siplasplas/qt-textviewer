//
// Created by Andrzej Borucki on 2022-10-04
//

#ifndef TEXTVIEWER_SELECTION_H
#define TEXTVIEWER_SELECTION_H

#include <cstdint>
#include <QPoint>
#include "../logic/ViewLogic.h"

struct Selection {
    int64_t selBegin; //inclusive
    int64_t selEnd; //exclusive
    bool charSelected(QPoint point, vl::ViewResult &vr);
};


#endif //TEXTVIEWER_SELECTION_H
