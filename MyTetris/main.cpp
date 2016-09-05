#include "stdafx.h"
#include "MyTetris.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	CTetrisModel model;
    MyTetris w(&model);
    w.show();
    return a.exec();
}
