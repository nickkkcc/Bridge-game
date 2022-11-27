#include "windows/entrywindow.h"
#include "ui_entrywindow.h"
EntryWindow::EntryWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EntryWindow)
{
    ui->setupUi(this);
    setupWindow();
    networkConnection = new ClientNetwork();
    connect(networkConnection,&ClientNetwork::connectionResult, this,&EntryWindow::connectionResult);
    connect(networkConnection,&ClientNetwork::loginResult, this,&EntryWindow::loginStatus);
    connect(networkConnection,&ClientNetwork::serverDisconnected, this,&EntryWindow::serverDisconnected);
    connect(networkConnection,&ClientNetwork::generalError, this,&EntryWindow::generalError);
    connect(this,&EntryWindow::connectToServer, networkConnection,&ClientNetwork::txRequestLogin);

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

void EntryWindow::on_enter_button_clicked()
{
    //тут такая конструкция для проверки на пустой логин
    //из-за того, что length работает с char и не дает прямо проверить qstring,приходиться делать костыль, может позже изменю
    int countL = ui->edit_LOGIN->text().length();
    int countS = 0;
    for(int i = 0; i < ui->edit_LOGIN->text().length();i++)
    {
        if (ui->edit_LOGIN->text().at(i) == ' ')
        {
            countS++;
        }
    }
    username = ui->edit_LOGIN->text();
    if (username == "" || countS == countL)
    {
        QMessageBox::warning(this,"Проблема","Ваше имя пользователя не может быть пустым!");
    }
    password = ui->passworld_edit->text();
    emit connectToServer(*ipAddress,portID,username,password);
    lobby *l = new lobby();
    l->show();

}


void EntryWindow::loginStatus(bool loginSuccessful, QString reason)
{
    if(loginSuccessful == false)
    {
        QMessageBox::warning(this,"Login status",reason);
    }
}

void EntryWindow::connectionResult(int status, QString errorMsg)
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
//        msg = "Already logged in. Nothing was changed.";
//        QMessageBox::warning(this,"Login failure",msg);
//        break;
    }
    default:
    {
        break;
    }
    }
}

void EntryWindow::serverDisconnected()
{
//    QMessageBox::warning(this,"Game started","You have not been picked and the game has started without you.");
//    this->close();
}

void EntryWindow::generalError(QString errorMsg)
{
    QMessageBox::warning(this,"Error message",errorMsg);
}
void EntryWindow::on_newAccount_clicked()
{
    NewAccount *newaccount = new NewAccount();
    newaccount->show();
}

//void EntryWindow::updateGameState(PlayerGameState player)
//{
//    if(player.getEvent() == INITIALIZE)
//    {
//        playerWindow = new GameWindow(networkConnection);
//        playerWindow->setName(username);
//        playerWindow->setGameState(player);
//        connect(networkConnection,&ClientNetwork::notifyBidTurn, playerWindow,&GameWindow::playerTurnBid);
//        disconnect(networkConnection,&ClientNetwork::connectionResult, this,&entrywindow::connectionResult);
//        disconnect(networkConnection,&ClientNetwork::loginResult, this,&entrywindow::loginStatus);
//        disconnect(networkConnection,&ClientNetwork::serverDisconnected, this,&entrywindow::serverDisconnected);
//        disconnect(networkConnection,&ClientNetwork::generalError, this,&entrywindow::generalError);
//        disconnect(networkConnection,&ClientNetwork::updateGameState, this,&entrywindow::updateGameState);
//        disconnect(this,&entrywindow::connectToServer, networkConnection,&ClientNetwork::txRequestLogin);
//        this->close();

//    }
//}



