#include "inc/socket.h"

Socket::Socket(QUrl url, QObject *parent) : QObject{parent}
{
    this->url = url;
    qInfo() << "Client try connect to server:" << url.toDisplayString();
    connect(&socket, &QWebSocket::connected, this, &Socket::onConnected);
    connect(&socket, &QWebSocket::disconnected, this, &Socket::onDisconnected);
}

void Socket::txJson(const QJsonObject txObj)
{
    QJsonDocument txDoc(txObj);
    socket.sendTextMessage(txDoc.toJson(QJsonDocument::Compact));
}

void Socket::rxAll(const QString &message)
{
    rx = QJsonDocument::fromJson(message.toUtf8()).object();
    if (!rx.isEmpty())
    {
        emit sendJson(rx);
    }
}

void Socket::onConnected()
{
    qInfo() << "Client connected to server:" << url.toDisplayString();
    connect(&pingTimer, &QTimer::timeout, this, &Socket::onPing);
    connect(&socket, &QWebSocket::pong, this, &Socket::pong);
    connect(&socket, &QWebSocket::textMessageReceived, this, &Socket::rxAll);
    pingTimer.start(500);
    emit sendConnected();
}

void Socket::onDisconnected()
{
    qWarning() << "client disconnected from server:" << url.toDisplayString();
    emit sendDisconnected();
}

void Socket::onPing()
{
    socket.ping();
}

void Socket::onCloseConnection()
{
    socket.abort();
    pingTimer.stop();
    qInfo() << "Client connected to server:" << url.toDisplayString();
    emit sendDisconnected();
}

void Socket::onConnectToServer()
{
    socket.open(QUrl(url));
}
