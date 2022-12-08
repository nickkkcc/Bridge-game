#include "windows/newaccount.h"
#include "ui_newaccount.h"

NewAccount::NewAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewAccount)
{
    ui->setupUi(this);
    networkConnection = new ClientNetwork();
    ui->comboBox->addItems(networkConnection->listq);
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
    question = ui->comboBox->currentIndex();
    login = ui->new_login->text();
    password = ui->new_password->text();
    answer = ui->comboBox->itemText(question);
    if (login == "" || countS == countL || password == "" || answer== "")
    {
        QMessageBox::warning(this,"Проблема","Поля пустые!");
    }
    else{
        networkConnection->sendRegistr(login,password,question,answer);
        NewAccount::close();
        qInfo("Данные переданы сети");
    }

}


