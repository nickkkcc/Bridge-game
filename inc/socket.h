#pragma once

#include "flags.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTimer>
#include <QWebSocket>

class Socket : public QObject
{

    Q_OBJECT
  public:
    explicit Socket(QUrl url, QObject *parent = nullptr);
  public slots:
    void txJson(const QJsonObject txObj);
    void rxAll(const QString &message);
    void onCloseConnection();
    void onConnectToServer();
  private slots:
    void onConnected();
    void onDisconnected();
    void onPing();

    // From UI

  signals:
    void sendConnected();
    void sendDisconnected();
    void pong(quint64 elapsedTime, const QByteArray &payload);
    void sendJson(const QJsonObject rxObj);

  private:
    QWebSocket socket;
    QUrl url;
    QTimer pingTimer;
    QUuid uuid;
    QJsonObject rx;
};
