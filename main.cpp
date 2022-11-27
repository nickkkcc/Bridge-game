#include "windows/entrywindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EntryWindow w;
    w.show();
    return a.exec();
}
