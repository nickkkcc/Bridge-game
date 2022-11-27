#ifndef ENTRYWINDOW_H
#define ENTRYWINDOW_H

#include <QWidget>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include "network/clientnetwork.h"
#include "newaccount.h"
#include "windows/lobby.h"
QT_BEGIN_NAMESPACE
namespace Ui { class EntryWindow; }
QT_END_NAMESPACE

class EntryWindow : public QWidget
{
    Q_OBJECT

public:
    EntryWindow(QWidget *parent = nullptr);
    ~EntryWindow();
    void setupWindow();
    quint16 portID = 8888;
    QString password;
    QHostAddress *ipAddress = new QHostAddress ("127.0.0.1"); //"31.134.130.194"
    QString username;
    int pageID = 3;
    bool loginSuccessful = false;
    ClientNetwork *networkConnection = nullptr;
    NewAccount *newaccount = nullptr;

public slots:
    void loginStatus(bool loginSuccessful, QString reason);
    void connectionResult(int status, QString errorMsg);
    void serverDisconnected();
    void generalError(QString errorMsg);

private slots:
    void on_enter_button_clicked();
    void on_newAccount_clicked();

private:
    Ui::EntryWindow *ui;


signals:
    void connectToServer(QHostAddress serverIP, quint16 port, QString playerName, QString password);



};
#endif // ENTRYWINDOW_H
