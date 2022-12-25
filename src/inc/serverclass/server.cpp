#include <QMessageBox>

#include "inc/network/servernetwork.h"
#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) : QMainWindow(parent), ui(new Ui::Server)
{

    ui->setupUi(this);
    serverNetwork = new ServerNetwork(this);

    connect(serverNetwork, &ServerNetwork::connectionResult, this, &Server::connectionResult);
    connect(serverNetwork, &ServerNetwork::generalError, this, &Server::generalError);
}

const ServerNetwork &Server::getServerNetwork() const
{

    return *serverNetwork;
}

void Server::tryConnect()
{

    if (ui->portLine->text() == "")
    {
        port = 8888;
    }
    else
    {
        port = ui->portLine->text().toUShort();
    }

    maxPlayers = ui->maxPlayersLine->text().toInt();
    maxLoginLength = ui->MaxLoginLengthLine->text().toInt();
    minLoginLength = ui->minLoginLength->text().toInt();

    serverNetwork->initServer(port, maxPlayers, maxLoginLength, minLoginLength);
}

void Server::connectionResult(int status, quint16 port, QString errorMsg)
{

    QString msg = "";
    switch(status)
    {
    case 1:
    {
        msg = "Порт " + QString::number(port) +" неверен";
                  QMessageBox::warning(this,"Неправильные настройки сервера",msg);
        break;
    }
    case 2:
    {
        msg = errorMsg;
        QMessageBox::warning(this,"Неправильные настройки сервера",msg);
        break;
    }
    case 3:
    {
        msg = "Сервер уже создан.";
        QMessageBox::warning(this,"Неправильная настройка сервера",msg);
        break;
    }
    case 4:
        msg = "Невозможно открыть базу данных.";
        QMessageBox::warning(this,"Неправильная настройка сервера",msg);
        break;
    default:
        break;
    }
}

void Server::generalError(QString errorMsg)
{

    QMessageBox::warning(this, "Сообщение об ошибке", errorMsg);
}

void Server::on_CreateBtn_clicked()
{

    if (!serverNetwork)
    {

        serverNetwork = new ServerNetwork(this);
    }
    ui->CreateBtn->setEnabled(false);
    ui->StopBtn->setEnabled(true);
    ui->RestartBtn->setEnabled(true);
    ui->DeleteAllClientBtn->setEnabled(true);
    tryConnect();
}

void Server::on_RestartBtn_clicked()
{

    delete serverNetwork;
    serverNetwork = nullptr;
    serverNetwork = new ServerNetwork(this);
    tryConnect();
}

void Server::on_StopBtn_clicked()
{

    delete serverNetwork;
    serverNetwork = nullptr;
    ui->CreateBtn->setEnabled(true);
    ui->StopBtn->setEnabled(false);
    ui->RestartBtn->setEnabled(false);
    ui->DeleteAllClientBtn->setEnabled(false);
}

void Server::on_DeleteAllClientBtn_clicked()
{

    serverNetwork->deleteAllClients();
}
