#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include "inc/network/servernetwork.h"
#include <QHostAddress>
#include <QObject>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QMainWindow
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    const ServerNetwork& getServerNetwork() const;


public slots:
    void closeServer();
private slots:
    //    void clientDisconnected();
    void on_CreateBtn_clicked();
    void on_RestartBtn_clicked();

    void tryConnect();
    void connectionResult(int status, quint16 port, QString errorMsg);
    void generalError(QString errorMsg);


    void on_StopBtn_clicked();

    void on_DeleteAllClientBtn_clicked();

private:
    ServerNetwork* serverNetwork = nullptr;
    Ui::Server *ui;
    QUrl ipAddress;
    ushort port;
    int maxLoginLength;
    int maxPlayers;
    int minLoginLength;
};
#endif // SERVER_H
