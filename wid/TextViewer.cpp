#include "TextViewer.h"

#include <QPainter>
#include <QKeyEvent>
#include <QFontDatabase>
#include <QHBoxLayout>
#include "utf/UTF.hpp"
#include <QDebug>

namespace wid {
    TextViewer::TextViewer(const char *addr, int64_t fileSize, QWidget *parent) :
            QWidget(parent) {
        setWindowTitle(tr("Viewer"));
        paintArea = new PaintArea(addr, fileSize, this);
        paintArea->setCursor(Qt::IBeamCursor);
        auto *vLayout = new QVBoxLayout;
        auto *hLayout = new QHBoxLayout;
        hscroll = new QScrollBar(this);
        hscroll->setOrientation(Qt::Horizontal);
        vscroll = new QScrollBar(this);
        hLayout->addWidget(paintArea);
        hLayout->addWidget(vscroll);
        vLayout->addLayout(hLayout);
        vLayout->addWidget(hscroll);
        setLayout(vLayout);
        connect(hscroll, &QScrollBar::valueChanged, this, &TextViewer::hscrollChanged);
    }

#if QT_CONFIG(wheelevent)

    void TextViewer::wheelEvent(QWheelEvent *event) {
        int delta = event->angleDelta().y() / 40;
        if (event->modifiers() == Qt::ShiftModifier)
            paintArea->wheelHorizontal(delta);
        else
            paintArea->wheelVertical(delta);
    }

    void TextViewer::hscrollChanged() {
        qDebug() << hscroll->value();
    }

    void TextViewer::mousePressEvent(QMouseEvent *event) {
        paintArea->setFocus();
    }

#endif
}