// Copyright (C) 2016 Kurt Pattyn <pattyn.kurt@gmail.com>.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>

class RegistrationClient : public QObject
{
    Q_OBJECT
public:
    explicit RegistrationClient(const QUrl &url, bool debug = false, QObject *parent = nullptr);

    enum Type{
        REGISTRATION_TRUE,
        REGISTRATION_FALSE,
        REGISTRATION_QUESTIONS,
        LOGIN_TRUE,
        LOGIN_FALSE,
        LOGIN_FALSE_NOSUCH_PASSWORD,
    };
    QStringList questions{
        "How Are You",
        "GOR",
        "DA"
    };
Q_SIGNALS:
    void closed();
    void send(const Type& type);


private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(const QString& message);
    void sendD(const Type& type);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;
    int currentType;
    QString name;

    QJsonObject createJsonD(const Type& name);
    void checkAnswerFromServer(const QJsonObject & rx);
};

#endif // ECHOCLIENT_H
