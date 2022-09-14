//
// Created by Andrzej Borucki on 2022-09-13
//

#include "dialog.h"

#include <QVBoxLayout>
#include "wid/TextViewer.h"
#include <QLineEdit>
#include <QPushButton>

Dialog::Dialog()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    //vmap.openFile("../wid/PaintArea.cpp");
    vmap.openFile("/home/andrzej/testy/bigfile.dat");
    auto *widget = new wid::TextViewer(vmap.getAddr(), vmap.getFileSize(), this);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(new QLineEdit);
    hLayout->addWidget(new QPushButton);
    mainLayout->addLayout(hLayout);
    mainLayout->addWidget(widget);
    setLayout(mainLayout);
    mainLayout->setMargin(3);
    resize(400,400);
}
