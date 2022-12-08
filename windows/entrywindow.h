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
    QString login;
    ClientNetwork *networkConnection = nullptr;
    NewAccount *newaccount = nullptr;

public slots:


private slots:
    void on_enter_button_clicked();
    void on_newAccount_clicked();

private:
    Ui::EntryWindow *ui;


signals:



};
#endif // ENTRYWINDOW_H
