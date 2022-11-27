#ifndef NEWACCOUNT_H
#define NEWACCOUNT_H

#include <QWidget>
#include "network/clientnetwork.h"
namespace Ui {
class NewAccount;
}

class NewAccount : public QWidget
{
    Q_OBJECT

public:
    explicit NewAccount(QWidget *parent = nullptr);
    ~NewAccount();
public slots:
    void loginStatus(bool loginSuccessful, QString reason);
    void connectionResult(int status, QString errorMsg);
    void serverDisconnected();
    void generalError(QString errorMsg);
private slots:
    void on_comboBox_activated(int index);
    void on_pushButton_clicked();
signals:
    void connectToServer(QHostAddress serverIP, quint16 port, QString playerName, QString password);

private:
    Ui::NewAccount *ui;
    ClientNetwork *networkConnection = nullptr;
    quint16 portID = 8888;
    QString password;
    QHostAddress *ipAddress = new QHostAddress ("31.134.130.194");
    QString username;
};

#endif // NEWACCOUNT_H
