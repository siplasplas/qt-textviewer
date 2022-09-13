//
// Created by Andrzej Borucki on 2022-09-13
//

#include <QPainter>
#include <QFontDatabase>
#include "PaintArea.h"

namespace wid {
    void PaintArea::paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.fillRect(rect(), Qt::white);
        if (vr.lines)
            for (int i = 0; i < vr.lines->size(); i++) {
                QRect R(0, i * 17, this->rect().width(), 17);
                QString qstr = QString::fromWCharArray(vr[i].c_str());
                painter.drawText(R, Qt::AlignLeft, qstr);
            }
    }

    PaintArea::PaintArea(const char *addr, int64_t fileSize, QWidget *parent) : QWidget(parent) {
        QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setPointSizeF(10);
        QFontMetricsF fm(font, this);
        qreal pixelsWide = fm.horizontalAdvance("0");
        qreal pixelsHigh = fm.height();
        this->setFont(font);
        vl = new vl::ViewLogic(addr, fileSize);
        vl->screenLineCount = 20;
        vl->screenLineLen = 40;
        vl->maxLineLen = 10000000;
        vr = vl->lines(0);
    }

    PaintArea::~PaintArea() {
        delete vl;
    }
}