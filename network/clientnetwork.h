#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <QWidget>
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonObject>
#include <QHostAddress>
#include <QCoreApplication>
#include <QMessageBox>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonArray>
namespace Ui {
class ClientNetwork;
}

class ClientNetwork : public QObject
{
    Q_OBJECT

public:
    explicit ClientNetwork(QObject *parent = nullptr);
    ~ClientNetwork();
    void abort();
    QStringList listq;
    void sendRegistr(QString login, QString password, qint64 question, QString answer );
    void sendLoginAndPassword(QString login, QString password);
private slots:
    void onConnected();
    void sendData(QJsonObject data);
    void serverResponse(const QString &message);
    void sendQuestion();
signals:
    void closed();

private:
    bool successful = false;
    QUrl url;
    QWebSocket m_webSocket;
    QString m_login;
    QString m_password;
    QString m_question;
    QString m_answer;
    QString error;
    QUuid id;
    qint64 token;
    QVariant list;



};

#endif // CLIENTNETWORK_H
