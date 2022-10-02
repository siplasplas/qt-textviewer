//
// Created by Andrzej Borucki on 2022-09-13
//

#include <QPainter>
#include <QFontDatabase>
#include <cmath>
#include <QElapsedTimer>
#include "PaintArea.h"

namespace wid {
    void PaintArea::paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.fillRect(rect(), Qt::white);
        QString pilcrow(1);
        pilcrow[0] = 182;
        QPen pen(Qt::black);
        painter.setPen(pen);
        if (vr.lines)
            for (int i = 0; i < vr.lines->size(); i++) {
                QRect R(0, i * fontHeight, this->rect().width(), fontHeight);
                QString qstr = QString::fromWCharArray(vr[i].c_str());
                if (i == vr.lines->size()-1 && qstr.isEmpty()) {
                    if (vr.lines->at(i).li->next==vl->fileSize) {
                        QPen pen1(Qt::gray);
                        painter.setPen(pen1);
                        painter.drawText(R, Qt::AlignLeft, pilcrow);
                    }
                } else
                    painter.drawText(R, Qt::AlignLeft, qstr);
            }
    }

    PaintArea::PaintArea(const char *addr, int64_t fileSize, QWidget *parent) : QWidget(parent) {
        QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setPointSizeF(10);
        QFontMetricsF fm(font, this);
        fontWidth = fm.horizontalAdvance("0");
        fontHeight = fm.height();
        this->setFont(font);
        vl = new vl::ViewLogic(addr, fileSize);
    }

    PaintArea::~PaintArea() {
        delete vl;
    }

    void PaintArea::resizeEvent(QResizeEvent *event) {
        vl->screenLineCount = ceil((double)event->size().height()/fontHeight);
        vl->screenLineLen = ceil((double)event->size().width()/fontWidth);
        vr = vl->lines(vl->fileSize, beginX);
    }

    void PaintArea::wheelVertical(int delta) {
        if (delta>0)
            vl->scrollNUp(delta, vr);
        else if (delta<0)
            vl->scrollNDown(-delta, vr);
        update();
    }

    void PaintArea::wheelHorizontal(int delta) {
        beginX = std::max(0, beginX-delta);
        vr = vl->lines(vl->fileSize, beginX);
        update();
    }
}