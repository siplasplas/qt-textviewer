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
//    void resizeEvent(QResizeEvent *event) override;
//    void keyPressEvent(QKeyEvent *event) override;
#if QT_CONFIG(wheelevent)

        void wheelEvent(QWheelEvent *event) override;

#endif
//    void mousePressEvent(QMouseEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
//    void mouseReleaseEvent(QMouseEvent *event) override;
    private:
//    void scroll(int deltaX, int deltaY);
    };

#endif // VIEWER_TEXTVIEWER_H
}