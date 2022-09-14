//
// Created by Andrzej Borucki on 2022-09-13
//

#include <QPainter>
#include <QFontDatabase>
#include <cmath>
#include <QElapsedTimer>
#include "PaintArea.h"
#include <QDebug>

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
        fontWidth = fm.horizontalAdvance("0");
        fontHeight = fm.height();
        this->setFont(font);
        vl = new vl::ViewLogic(addr, fileSize);
    }

    PaintArea::~PaintArea() {
        delete vl;
    }

    void PaintArea::resizeEvent(QResizeEvent *event) {
        vl->screenLineCount = ceil(event->size().height()/fontHeight);
        vl->screenLineLen = ceil(event->size().width()/fontWidth);
        vl->maxLineLen = 1000000;
        QElapsedTimer timer;
        timer.start();
        vr = vl->lines(vl->fileSize);
        qDebug() << timer.nsecsElapsed()/1e6;
    }
}