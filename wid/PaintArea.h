//
// Created by Andrzej Borucki on 2022-09-13
//

#ifndef VIEWER_PAINTAREA_H
#define VIEWER_PAINTAREA_H

#include <QWidget>
#include <QPaintEvent>
#include <QTimer>
#include "logic/ViewLogic.h"
#include "misc/Selection.h"

namespace wid {
    class PaintArea : public QWidget {
        Q_OBJECT
        vl::ViewLogic *vl;
        int beginX = 0;
        qreal fontWidth, fontHeight;
        QTimer timer;
        bool drawCaret = true;
        Selection selection;
        QPoint caretPos;
        void doBlinkMethod();
        void drawSelBackground(QPainter &painter, int row);
        static QColor getSelColor();
        QChar updateCaretPos();
    protected:
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
#if QT_CONFIG(wheelevent)
        void wheelEvent(QWheelEvent *event) override;
#endif
    public:
        explicit PaintArea(const char *addr, int64_t fileSize, QWidget *parent);
        ~PaintArea() override;
        void wheelVertical(int delta);
        void wheelHorizontal(int delta);
        void trySetCaret(QPoint point);
        void setHorizontal(int beginX);
        void setVertical(int64_t position);
        vl::ViewResult vr;
        int screenLineCount;
        int screenLineLen;
    Q_SIGNALS:
        void sizeChanged();
    };
}

#endif //VIEWER_PAINTAREA_H
