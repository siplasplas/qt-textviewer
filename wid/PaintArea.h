//
// Created by Andrzej Borucki on 2022-09-13
//

#ifndef VIEWER_PAINTAREA_H
#define VIEWER_PAINTAREA_H

#include <QWidget>
#include <QPaintEvent>
#include "logic/ViewLogic.h"

namespace wid {
    class PaintArea : public QWidget {
        vl::ViewLogic *vl;
        vl::ViewResult vr;
        qreal fontWidth, fontHeight;
    protected:
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
    public:
        explicit PaintArea(const char *addr, int64_t fileSize, QWidget *parent);
        ~PaintArea() override;
    };
}

#endif //VIEWER_PAINTAREA_H
