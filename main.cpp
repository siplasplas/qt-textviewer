//
// Created by Andrzej Borucki on 2022-09-13
//

#include "wid/TextViewer.h"

#include <QApplication>
#include <dialog.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Dialog dialog;
    dialog.show();
    return app.exec();
}
