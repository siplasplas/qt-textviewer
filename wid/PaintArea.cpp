//
// Created by Andrzej Borucki on 2022-09-13
//

#include <QPainter>
#include <QFontDatabase>
#include <cmath>
#include <QElapsedTimer>
#include <QDebug>
#include "PaintArea.h"

namespace wid {
    void PaintArea::drawSelBackground(QPainter &painter, int row) {
        int selColBeg = selection.selColBeg(row, vr);
        int selColEnd = selection.selColEnd(row, vr);
        if (selColBeg==-1) {
            if (selColEnd==-1)
                painter.fillRect(0,row*fontHeight, QWidget::width(),(row+1)*fontHeight, Qt::white);
            else
                assert(false);
        } else if (selColBeg==0) {
            if (selColEnd==-1)
                painter.fillRect(0,row*fontHeight, QWidget::width(),(row+1)*fontHeight, getSelColor());
            else {
                painter.fillRect(0,row*fontHeight, selColEnd*fontWidth,(row+1)*fontHeight, getSelColor());
                painter.fillRect(selColEnd*fontWidth,row*fontHeight, QWidget::width()-selColEnd*fontWidth,(row+1)*fontHeight, Qt::white);
            }
        } else {
            if (selColEnd==-1) {
                painter.fillRect(0,row*fontHeight, selColBeg*fontWidth,(row+1)*fontHeight, Qt::white);
                painter.fillRect(selColBeg*fontWidth,row*fontHeight, QWidget::width()-selColBeg*fontWidth,(row+1)*fontHeight, getSelColor());
            } else {
                painter.fillRect(0,row*fontHeight, selColBeg*fontWidth,(row+1)*fontHeight, Qt::white);
                painter.fillRect(selColBeg*fontWidth,row*fontHeight, (selColEnd-selColBeg)*fontWidth,(row+1)*fontHeight, getSelColor());
                painter.fillRect(selColEnd*fontWidth,row*fontHeight, QWidget::width()-selColEnd*fontWidth,(row+1)*fontHeight, getSelColor());
            }
        }
    }

    void PaintArea::paintEvent(QPaintEvent *event) {
        selection.compute(vr);
        QPainter painter(this);
        painter.setRenderHint(QPainter::TextAntialiasing);
        QRect R = event->rect();
        bool oneCharRepaint = R.width()==ceil(fontWidth);
        QString pilcrow(1);
        pilcrow[0] = 182;
        QPen pen(Qt::black);
        pen.setWidth(1);
        painter.setPen(pen);
        if (vr.lines) {
            QChar qchar = updateCaretPos();
            if (oneCharRepaint) {
                if (selection.charSelected(caretPos, vr))
                    painter.fillRect(R, getSelColor());
                else
                    painter.fillRect(R, Qt::white);
                painter.drawText(R, Qt::AlignLeft, qchar);
            } else {
                for (int i = 0; i < vr.lines->size(); i++) {
                    drawSelBackground(painter, i);
                    QRect R(0, i * fontHeight, this->rect().width(), fontHeight);
                    QString qstr = QString::fromWCharArray(vr[i].c_str());
                    if (i == vr.lines->size() - 1 && qstr.isEmpty()) {
                        if (vr.lines->at(i).li->next == vl->fileSize) {
                            QPen pen1(Qt::gray);
                            painter.setPen(pen1);
                            painter.drawText(R, Qt::AlignLeft, pilcrow);
                        }
                    } else
                        painter.drawText(R, Qt::AlignLeft, qstr);
                }
                int y = vr.lines->size()*fontHeight;
                painter.fillRect(0,y, QWidget::width(), QWidget::height()-y, Qt::white);
            }
        }
        if (drawCaret && hasFocus()) {
            QPen pen(Qt::black);
            pen.setWidth(2);
            painter.setPen(pen);
            int x = (int)(caretPos.x()*fontWidth+1);
            int y = (int)(caretPos.y()*fontHeight);
            painter.drawLine(x, y+1, x, y+fontHeight-1);
        }
    }

    QChar PaintArea::updateCaretPos() {
        if (caretPos.y() >= vr.lines->size()) {
            caretPos.setY(vr.lines->size() - 1);
            caretPos.setX(width() / fontWidth);
        }
        QString qstr = QString::fromWCharArray(vr[caretPos.y()].c_str());
        QChar qchar;
        if (qstr.length() <= caretPos.x()) {
            caretPos.setX(qstr.length());
            qchar = ' ';
        } else qchar = qstr[caretPos.x()];
        return qchar;
    }

    QColor PaintArea::getSelColor() {
        QColor selColor(0xa6, 0xd2, 0xff);
        return selColor;
    }

    PaintArea::PaintArea(const char *addr, int64_t fileSize, QWidget *parent) : QWidget(parent) {
        selection.selBegin = 0x31;
        selection.selEnd = 0x79;
        QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setPointSizeF(10.5);
        QFontMetricsF fm(font, this);
        fontWidth = fm.horizontalAdvance("0");
        fontHeight = fm.height();
        this->setFont(font);
        vl = new vl::ViewLogic(addr, fileSize);
        connect(&timer, &QTimer::timeout, this, &PaintArea::doBlinkMethod);
    }

    PaintArea::~PaintArea() {
        delete vl;
    }

    void PaintArea::resizeEvent(QResizeEvent *event) {
        screenLineCount = ceil((double)event->size().height()/fontHeight);
        vl->screenLineCount = screenLineCount;
        vl->screenLineLen = ceil((double)event->size().width()/fontWidth);
        vl->lo->update();
        vr = vl->lines();
        sizeChanged();
    }

    void PaintArea::wheelVertical(int delta) {
        if (delta>0)
            vl->scrollNUp(delta, vr);
        else if (delta<0)
            vl->scrollNDown(-delta, vr);
        update();
    }

    void PaintArea::setHorizontal(int beginX) {
        vl->lo->beginX = beginX;
        vr = vl->lines();
        update();
    }

    void PaintArea::setVertical(int64_t position) {
        vl->lo->gotoProportional(position, vl->lo->beginX);
        vr = vl->lines();
        update();
    }

    void PaintArea::wheelHorizontal(int delta) {
        beginX = std::max(0, beginX-delta);
        vl->lo->beginX = beginX;
        vr = vl->lines();
        update();
    }

    void PaintArea::doBlinkMethod() {
        drawCaret = !drawCaret;
        int x = caretPos.x()*fontWidth;
        int y = caretPos.y()*fontHeight;
        repaint(x, y, ceil(fontWidth), ceil(fontHeight));
    }

    void PaintArea::mousePressEvent(QMouseEvent *event) {
        setFocus();
        trySetCaret(event->pos());
    }

#if QT_CONFIG(wheelevent)
    void PaintArea::wheelEvent(QWheelEvent *event) {
        int delta = event->angleDelta().y() / 40;
        if (event->modifiers() == Qt::ShiftModifier)
            wheelHorizontal(delta);
        else
            wheelVertical(delta);
    }

    //set caret on char containing point or on row and end of line
    void PaintArea::trySetCaret(QPoint point) {
        caretPos.setX(int(point.x()/fontWidth));
        caretPos.setY(int(point.y()/fontHeight));
        timer.start(500);
        update();
    }

#endif
}
