#include "windows/newaccount.h"
#include "ui_newaccount.h"

NewAccount::NewAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewAccount)
{
    ui->setupUi(this);
    networkConnection = new ClientNetwork();
    connect(networkConnection,&ClientNetwork::connectionResult, this,&NewAccount::connectionResult);
    connect(networkConnection,&ClientNetwork::loginResult, this,&NewAccount::loginStatus);
    connect(networkConnection,&ClientNetwork::serverDisconnected, this,&NewAccount::serverDisconnected);
    connect(networkConnection,&ClientNetwork::generalError, this,&NewAccount::generalError);
    connect(this,&NewAccount::connectToServer, networkConnection,&ClientNetwork::txRequestLogin);
}

NewAccount::~NewAccount()
{
    delete ui;
}

void NewAccount::on_comboBox_activated(int index)
{
    if(index > 0){
        ui->secretQues_line->setEnabled(true);
    }
}


void NewAccount::on_pushButton_clicked()
{
    int countL = ui->new_login->text().length();
    int countS = 0;
    for(int i = 0; i < ui->new_login->text().length();i++)
    {
        if (ui->new_login->text().at(i) == ' ')
        {
            countS++;
        }
    }
    username = ui->new_login->text();
    if (username == "" || countS == countL)
    {
        QMessageBox::warning(this,"Проблема","Ваше имя пользователя не может быть пустым!");
    }
    password = ui->new_password->text();
    emit connectToServer(*ipAddress,portID,username,password);
}


void NewAccount::loginStatus(bool loginSuccessful, QString reason)
{
    if(loginSuccessful == false)
    {
        QMessageBox::warning(this,"Login status",reason);
    }
}

void NewAccount::connectionResult(int status, QString errorMsg)
{
    QString msg;
    switch(status)
    {
    case 0:
    {
        break;
    }
    case 1:
    {
        QMessageBox::warning(this,"Login failure",errorMsg);
        break;
    }
    case 3:
    {
        msg = "Already logged in. Nothing was changed.";
        QMessageBox::warning(this,"Login failure",msg);
        break;
    }
    default:
    {
        break;
    }
    }
}

void NewAccount::serverDisconnected()
{
    QMessageBox::warning(this,"Game started","You have not been picked and the game has started without you.");
    this->close();
}

void NewAccount::generalError(QString errorMsg)
{
    QMessageBox::warning(this,"Error message",errorMsg);
}


