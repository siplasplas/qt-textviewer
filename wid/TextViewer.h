#ifndef VIEWER_TEXTVIEWER_H
#define VIEWER_TEXTVIEWER_H

#include <QWidget>
#include <QScrollBar>
#include "PaintArea.h"

namespace wid {
    class TextViewer : public QWidget {
    Q_OBJECT
        QScrollBar *vscroll;
        QScrollBar *hscroll;
        PaintArea *paintArea;
    public:
        explicit TextViewer(const char *addr, int64_t fileSize, QWidget *parent = nullptr);
    protected:
//    void keyPressEvent(QKeyEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
//    void mouseReleaseEvent(QMouseEvent *event) override;
    private:
        const int MAXVSCROLL = 10000;
        int64_t fileSize;
        void hscrollChanged();
        void vscrollChanged();
        void sizeChanged();
        void setHScroll();
    };

#endif // VIEWER_TEXTVIEWER_H
}