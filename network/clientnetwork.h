#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <QWidget>
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QJsonObject>
#include <QHostAddress>
#include <QDateTime>
#include <QCoreApplication>
#include <QMessageBox>
namespace Ui {
class ClientNetwork;
}

class ClientNetwork : public QWidget
{
    Q_OBJECT

public:
    explicit ClientNetwork(QWidget *parent = nullptr);
    ~ClientNetwork();
    void abort(); // Only used for testing.

public slots:
    void txRequestLogin(QHostAddress serverIP, quint16 port, QString playerName, QString password);

private slots:
    void rxAll();
    void internalServerDisconnected(); // Notice the name change between this and the signal's name in the Group design doc.
    void socketConnected(); // Notice that this function is not in UML diagram.
    void socketError(QAbstractSocket::SocketError socError);

signals:
    void connectionResult(int status, QString errorMsg);
    // status = 0 :connection was successful
    // status = 1 :connection was not successful, since the IP address or/and port are invalid. (See note regarding errorMsg.)
    // status = 3 :already connected, old connection was kept. (Nothing was changed.)
    // errorMsg is empty except when status = 1, then the actual error will be displayed.
    // GUI is responsible for creating the messages regarding connection status. (generalError signals will not be used for this.)

    void generalError(QString errorMsg); // All errors. (Should be displayed to the player.)
    // When HostNotFoundError or ConnectionRefusedError occurs (basically when connecting to the host was unsuccessful),
    // the connectionResult(1,errorMsg) is emited instead.
    // generalError not emited when server unexpectedly disconnects, rather serverDisconnected or gameTerminated is emited.

    void notifyBidTurn();
    void notifyMoveTurn();
    void notifyBidRejected(QString reason);
    void notifyMoveRejected(QString reason);
    void loginResult(bool loginSuccessful, QString reason);
    void serverDisconnected();
    // Only used before a game has been started, all terminations during a game will happen through gameTerminated().
    // When the game is started and this client has not been added to the game, the server will disconnect the client.
    // The ClientNetwork will then emit serverDisconnected. The client should then be terminated.

    void gameTerminated(QString reason);
    // This can be emmited anytime after a game has been started.
    // If client lost connection to the server while in a game, the reason given with gameTerminated will be: "Client lost connection to the server."
    // If another reason was given, it meant that the server terminated the game.
    // The Client (and probably the server) must be RESTARTED, before attempting to connect again.
    // The GUI is responsible for ensuring that the client is RESTARTED. (Do not go back to login page.)

private:
    void txAll(QJsonObject data);
    void rxNotifyBidTurn();
    void rxNotifyMoveTurn();
    void rxNotifyBidRejected(QJsonObject reasonObj);
    void rxNotifyMoveRejected(QJsonObject reasonObj);
    void rxLoginResult(QJsonObject resObj);
    void rxUpdateGameState(QJsonObject gsObj);
    void rxMessage(QJsonObject msgObj);
    void rxGameTerminated(QJsonObject reasonObj);

    QString playerName;
    QString tempPlayerName;
    QString tempPassword;
    bool bLoggedIn;
    QTcpSocket* tcpSoc;
    bool gameStarted;
    QDataStream in;
    qint64 idCounter;
    qint64 prevID;
    bool gameTerminatedOnce;
};

#endif // CLIENTNETWORK_H
