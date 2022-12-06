#include "inc/serverclass/server.h"

#include <QApplication>
#include <QWebSocketServer>
#include <QWebSocket>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server w;
    w.show();
    return a.exec();
}
