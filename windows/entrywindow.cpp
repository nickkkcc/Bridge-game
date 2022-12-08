#include "windows/entrywindow.h"
#include "ui_entrywindow.h"
EntryWindow::EntryWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EntryWindow)
{
    ui->setupUi(this);
    setupWindow();



}

EntryWindow::~EntryWindow()
{
    delete ui;
}

void EntryWindow::setupWindow()
{
    QPixmap bkgnd(":/res/fonName");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);
    this->setFixedSize(640,480); //пока оставил размер окна таким, потом подумаем насчет него
    this->setWindowTitle ("Rubber Contract Bridge");
    ui->login_label->setStyleSheet("color: rgb(255, 255, 255)");
    ui->password_label->setStyleSheet("color: rgb(255, 255, 255)");

}

void EntryWindow::on_enter_button_clicked() //кнопка входа
{

    int countL = ui->edit_LOGIN->text().length();
    int countS = 0;
    for(int i = 0; i < ui->edit_LOGIN->text().length();i++)
    {
        if (ui->edit_LOGIN->text().at(i) == ' ')
        {
            countS++;
        }
    }
    login = ui->edit_LOGIN->text();
    password = ui->passworld_edit->text();
    if (login == "" || countS == countL || password == "")
    {
        QMessageBox::warning(this,"Проблема","Поля пустые!");
    }
    networkConnection = new ClientNetwork();


}



void EntryWindow::on_newAccount_clicked()
{
    NewAccount *newaccount = new NewAccount();
    newaccount->show();
}

