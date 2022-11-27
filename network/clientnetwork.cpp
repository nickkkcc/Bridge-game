#include "clientnetwork.h"


#include <network/clientnetwork.h>

ClientNetwork::ClientNetwork(QWidget *parent) :
    QWidget(parent)
{
    gameStarted = false;
    playerName = "";
    tcpSoc = nullptr;
    tempPlayerName = "";
    tempPassword = "";
    bLoggedIn = false;
    idCounter = 0;
    prevID = -1; // First ID received from server is 0. prevID should be smaller, thus -1.
    gameTerminatedOnce = false; // Only send gameTerminated once to the client. Set to true after gameTerminated has been sent once.
    tcpSoc = new QTcpSocket(this);

    in.setDevice(tcpSoc);
    in.setVersion(QDataStream::Qt_5_10);

    // Create connections
    // All reception
    connect(tcpSoc, &QIODevice::readyRead, this, &ClientNetwork::rxAll);
    // Connection errors
    connect(tcpSoc, &QAbstractSocket::errorOccurred, this, &ClientNetwork::socketError);
    // Sucsessfully connected
    connect(tcpSoc, &QAbstractSocket::connected, this, &ClientNetwork::socketConnected);
}

ClientNetwork::~ClientNetwork()
{
    if (tcpSoc != nullptr){
        tcpSoc->abort();
        tcpSoc->deleteLater();
    }
}

void ClientNetwork::abort()
{
    if (tcpSoc != nullptr){
        tcpSoc->abort();
    }
}

void ClientNetwork::txRequestLogin(QHostAddress serverIP, quint16 port, QString playerName, QString password)
{
    // Connect to the server.
    // Cannot Request a new login while connected to the host?
    if (tcpSoc->state() != QAbstractSocket::UnconnectedState){
        emit connectionResult(3, "");
        return;
    }

    // Store playerName and password for use after port connected.
    tempPlayerName = playerName;
    tempPassword = password;

    // If connection has already been made, abort it.
    tcpSoc->abort();
    tcpSoc->connectToHost(serverIP, port);

    // socketConnected or socketError will be signaled next, depending on connectToHost()'s outcome.

    qInfo() << "txRequestLogin: Attempt to connect to the host with server IP: " << serverIP.toString() << " and port: " << port;

}



void ClientNetwork::rxAll()
{
    qInfo() << "rxAll Entered";

    in.startTransaction();

    QJsonObject rxObj;
    in >> rxObj;

    if (!in.commitTransaction()){
        emit generalError("Datastream read error occured. It is suggested to restart the game.");
        qInfo() << "rxAll: Datastream error occured.";
        return;
    }

    // Validate the QJsonObject
    // It should contain a Type field, with valid information in the string part.
    if (rxObj.contains("Type") && rxObj["Type"].isString() && rxObj.contains("ID") && rxObj["ID"].isDouble()){
        // QJsonObject received contained the expected data.
        // Test if ID number is larger than prevID
        if (rxObj["ID"].toInt() <= prevID) {
            emit generalError("Outdated data was received. The data will be ignored.");
            return;
        }

    } else {
        // QJsonObject received had errors (received data will be ignored).
        emit generalError("Data received from server has been incorrectly formatted. It is suggested to restart the game.");
        qInfo() << "rxAll: Data received from server has been incorrectly formatted.";
        return;
    }

    // The data received is valid.
    // Choose the function that will handel the data.
    QString tempStr = rxObj["Type"].toString();
    // Update the prevID
    prevID = rxObj["ID"].toInt();
    if (tempStr == "LOGIN_RESULT") {
        rxLoginResult(rxObj);
        return;
    }
    // Default
    emit generalError("An incorrect 'Type' has been received. The received data will be ignored.");
    return;

}

void ClientNetwork::internalServerDisconnected()
{
    // TODO: Return player name. Look at disconnection and when game has been started. (Setting the bool for that. - Not in this function.)

    if (gameStarted){
        qInfo() << "internalServerDisconnected: Client: " + playerName + " emitted gameTerminated.";

        if (gameTerminatedOnce == false){
            // Let the GUI know that connection to the server has been lost.
            emit gameTerminated("Client lost connection to the server.");
        }
        // Update relevant variables (Not complete list, since Client should be restarted.)
        gameStarted = false;

    } else {
        // The game has not yet been started.
        qInfo() << "internalServerDisconnected: Client: " + playerName + " emitted serverDisconnected.";
        emit serverDisconnected();
    }

    // Reset variables
    bLoggedIn = false;
    playerName = "";

    // Ensure that the client has been disconnected from the server.
    tcpSoc->abort();
}

void ClientNetwork::socketConnected()
{
    // Connection was sucessfull
    emit connectionResult(0, "");
    QMessageBox::warning(this,"Коннект","Коннект есть");
    // The first packet received by the server must have an ID of 0.
    idCounter = 0;
    // First packer sent from the client should have an ID of 0 and must be larger than prevID.
    prevID = -1;

    // Create QJsonObject
    QJsonObject txObj;
    txObj["Type"] = "LOGIN_REQUEST";
    txObj["Password"] = tempPassword;
    txObj["Alias"] = tempPlayerName;

    // Send the login request to the server
    txAll(txObj);
}


void ClientNetwork::socketError(QAbstractSocket::SocketError socError)
{

    qInfo() << "socketError: A socket error (on the client) occured: " << socError;
     QMessageBox::warning(this,"Ошибка","Ошибка подключения");
    // Determine the error
//    switch (socError) {
//    case QAbstractSocket::HostNotFoundError:
//        emit connectionResult(1, "The host was not found. Please check the host IP address and port settings.");
//        break;

//    case QAbstractSocket::ConnectionRefusedError:
//        emit connectionResult(1, "The connection was refused by the server. "
//                            "Make sure the server is running, "
//                            "and check that the host IP address and port settings are correct.");
//        break;

//        // If the host disconnects from the client.
//    case QAbstractSocket::RemoteHostClosedError:
//        internalServerDisconnected();
//        break;


//    default:
//        emit connectionResult(1, "The following error occurred: " + tcpSoc->errorString());
//    }

    tcpSoc->abort();
}



void ClientNetwork::txAll(QJsonObject data)
{
    // Cannot send data if not connected to the server.
    if (!tcpSoc->isValid()){
        emit generalError("Not connected to the server. Cannot send data to the server.");
        return;
    }

    // Test to see if data that must be sent is valid
    if (!data.contains("Type")){
        emit generalError("Data to be sent does not contain 'Type' field. Data was not sent.");
        return;
    }

    if (data.contains("ID")) {
        emit generalError("Data to be sent should not contain an 'ID' field. 'ID' field was removed.");
        data.remove("ID");
    }

    // Add the ID field to the QJsonObject
    data["ID"] = idCounter++;

    // Create transmitting communication data stream
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    // Add delay to give time for previous data sent to be received by the server
    qint64 timeNow = QDateTime::currentMSecsSinceEpoch() + 200; // Add 100ms
    while(timeNow > QDateTime::currentMSecsSinceEpoch()){
         QCoreApplication::processEvents(QEventLoop::AllEvents);
    }

    // Send the login request to the server
    out << data;
    int tempVal = tcpSoc->write(block);
    tcpSoc->waitForBytesWritten(tempVal);

    qInfo() << "txAll: Number of bytes sent to server: " << tempVal;

    if (tempVal == -1 || tempVal < block.size()) {
        // An error occured when writing the data block.
        // OR The block written was too small (did not contain enough bytes).
        emit generalError("An error occured with sending data to the server. It is suggested to restart the game.");
    }
}

void ClientNetwork::rxLoginResult(QJsonObject resObj)
{
    // The client should not already be logged in.
    if (bLoggedIn){
        emit generalError("The client is already logged in, but a login result message has been received. The login result from the server will be ignored.");
        return;
    }

    // The object validation has been done.
    if (!resObj.contains("loginSuccessful") || !resObj["loginSuccessful"].isBool() || !resObj.contains("reason") || !resObj["reason"].isString()) {
        emit generalError("Data received from server has been incorrectly formatted. It is suggested to restart the game.");
        return;
    }

    // Set the login flag accordingly.
    bLoggedIn = resObj["loginSuccessful"].toBool();

    // Notify client GUI of login result.
    emit loginResult(bLoggedIn, resObj["reason"].toString());

    // If login was unsuccessful, disconnect from the host.
    if (!bLoggedIn){
        tcpSoc->abort();
        qInfo() << "rxLoginResult: Login was unseccessfull and client is aborting connection with host.";
    } else {
        // If login was successful, set the playerName chosen.
        this->playerName = tempPlayerName;
    }
}




