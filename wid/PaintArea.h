//
// Created by Andrzej Borucki on 2022-09-13
//

#ifndef VIEWER_PAINTAREA_H
#define VIEWER_PAINTAREA_H

#include <QWidget>
#include <QPaintEvent>
#include <QTimer>
#include "logic/ViewLogic.h"

namespace wid {
    class PaintArea : public QWidget {
        vl::ViewLogic *vl;
        vl::ViewResult vr;
        int beginX = 0;
        qreal fontWidth, fontHeight;
        QTimer timer;
        bool drawCaret = true;
        QPoint caretPos;
        void doBlinkMethod();
    protected:
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
    public:
        explicit PaintArea(const char *addr, int64_t fileSize, QWidget *parent);
        ~PaintArea() override;
        void wheelVertical(int delta);
        void wheelHorizontal(int delta);
    };
}

#endif //VIEWER_PAINTAREA_H
