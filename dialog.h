//
// Created by Andrzej Borucki on 2022-09-13
//

#ifndef VIEWER_DIALOG_H
#define VIEWER_DIALOG_H

#include <QDialog>
#include "FileMap.h"

class Dialog : public QDialog
{
    FileMap vmap;
public:
    Dialog();
};

#endif // VIEWER_DIALOG_H
