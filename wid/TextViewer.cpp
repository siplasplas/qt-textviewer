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
        this->fileSize = fileSize;
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
        connect(vscroll, &QScrollBar::valueChanged, this, &TextViewer::vscrollChanged);
    }

    void TextViewer::hscrollChanged() {
        paintArea->setHorizontal(hscroll->value());
    }

    void TextViewer::vscrollChanged() {
        long double relative = (long double)(vscroll->value())/vscroll->maximum();
        int64_t position = int64_t(relative*fileSize);
        paintArea->setVertical(position);
    }
}