#include <QCoreApplication>
#include "createlobbytestclient.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    RegistrationClient client(QUrl(QStringLiteral("ws://31.134.130.194:8888")), true);
    CreateLobbyTest client(QUrl(QStringLiteral("ws://31.134.130.194:8888")), true);
    return a.exec();
}
