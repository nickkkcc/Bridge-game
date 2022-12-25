#include <QCoreApplication>
#include "createlobbytestclient.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    RegistrationClient client(QUrl(QStringLiteral("ws://31.134.129.26:8888")), true);
    CreateLobbyTest client(QUrl(QStringLiteral("ws://31.134.129.26:8888")), true, nullptr, "Kolya", 1);
    // CreateLobbyTest client2(QUrl(QStringLiteral("ws://31.134.129.26:8888")), true, nullptr, "Kolya", 2);
    return a.exec();
}
