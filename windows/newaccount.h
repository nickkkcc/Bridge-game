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

private slots:
    void on_comboBox_activated(int index);
    void on_pushButton_clicked();
signals:


private:
    Ui::NewAccount *ui;
    ClientNetwork *networkConnection = nullptr;
    QString login;
    QString password;
    QString answer;
    qint64 question;

};

#endif // NEWACCOUNT_H
