#pragma once

#include "flags.h"
#include "inc/findlobbymanager.h"
#include "inc/playerslist.h"
#include "socket.h"
#include <QObject>
#include <QWebSocket>

class MainClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList questions READ questions WRITE setQuestions NOTIFY questionsChanged)
    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorChanged)
    Q_PROPERTY(bool status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(bool statusVisible READ getStatusVisible WRITE setStatusVisible NOTIFY statusVisibleChanged)
    Q_PROPERTY(quint64 ping READ ping WRITE setPing NOTIFY pingChanged)
    Q_PROPERTY(PlayersList *playersList READ playersList WRITE setPlayersList NOTIFY playersListChanged)
    Q_PROPERTY(bool lobbyCreated READ getLobbyCreated WRITE setLobbyCreated NOTIFY lobbyCreatedChanged)
    Q_PROPERTY(QStringList TeamSelect READ TeamSelect WRITE setTeamSelect NOTIFY TeamSelectChanged)
    Q_PROPERTY(QString lobbyUud READ lobbyUud WRITE setLobbyUud NOTIFY lobbyUudChanged)
    Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged)
    Q_PROPERTY(
        int players_lobby_count READ players_lobby_count WRITE setPlayers_lobby_count NOTIFY players_lobby_countChanged)
    Q_PROPERTY(FindLobbyManager *findLobbyManager READ findLobbyManager WRITE setFindLobbyManager NOTIFY
                   findLobbyManagerChanged)
    Q_PROPERTY(QStringList cardPreView READ getCardPreView WRITE setCardPreView NOTIFY cardPreViewChanged)

  public:
    explicit MainClient(QObject *parent = nullptr);

    QStringList questions() const;
    void setQuestions(const QStringList &newQuestions);

    QString error() const;
    void setError(const QString &newError);

    bool status() const;
    void setStatus(bool newStatus);

    bool getStatusVisible() const;
    void setStatusVisible(bool newStatusVisible);

    quint64 ping() const;
    void setPing(quint64 newPing);

    PlayersList *playersList() const;
    void setPlayersList(PlayersList *newPlayersList);

    bool getLobbyCreated() const;
    void setLobbyCreated(bool newLobbyCreated);

    QStringList TeamSelect() const;
    void setTeamSelect(const QStringList &newTeamSelect);

    QString lobbyUud() const;
    void setLobbyUud(const QString &newLobbyUud);

    QString uuid() const;
    void setUuid(const QString &newUuid);

    int players_lobby_count() const;
    void setPlayers_lobby_count(int newPlayers_lobby_count);

    FindLobbyManager *findLobbyManager() const;
    void setFindLobbyManager(FindLobbyManager *newFindLobbyManager);

    QStringList getCardPreView() const;
    void setCardPreView(const QStringList &newCardPreView);

  public slots:

    Q_INVOKABLE void sendRegisterQuestions();
    Q_INVOKABLE void sendRegister(QString login, QString password, int index, QString answer);
    Q_INVOKABLE void sendLogin(QString login, QString password);
    Q_INVOKABLE void closeFromServer();
    Q_INVOKABLE bool questionsIsEmpty();
    Q_INVOKABLE void connectToServer();
    Q_INVOKABLE void createLobby(int team);
    Q_INVOKABLE void selectTeamAdmin(int team);
    Q_INVOKABLE void exitLobby();
    Q_INVOKABLE void exitInviterLobby();
    Q_INVOKABLE void addFriend(int index);
    Q_INVOKABLE void deleteFriend(int index);
    Q_INVOKABLE void clearPlayersList();
    Q_INVOKABLE void invitePlayer(int index);
    Q_INVOKABLE void inviteFriend(int index);
    Q_INVOKABLE void startGame();
    Q_INVOKABLE void findGame();
    Q_INVOKABLE void stopFindGame();
    Q_INVOKABLE void acceptedInvite(bool anwer);
    Q_INVOKABLE void selectTeam(int index);
    Q_INVOKABLE void loadImages();

  signals:

    void sendJson(const QJsonObject txJson);
    void gameStarted();
    void sendConnected();
    void sendDisconnected();
    void sendLoginAnswer();
    void questionsChanged();
    void errorChanged();
    void statusChanged();
    void statusVisibleChanged();
    void pingChanged();

    void playersListChanged();

    void lobbyCreatedChanged();

    void TeamSelectChanged();

    void lobbyUudChanged();

    void uuidChanged();

    void players_lobby_countChanged();

    void findLobbyManagerChanged();

    void cardPreViewChanged();

  private slots:
    void parseJson(const QJsonObject &rxObj);
    void sendPong(quint64 time);

  private:
    QJsonObject getTx(net::AnswerType type);
    void statusReset();

  private:
    FindLobbyManager *m_findLobbyManager;
    Socket *m_socket;
    QString m_uuid;
    QString m_lobbyUud;
    QStringList m_questions;
    QStringList cardPreView;
    bool m_status = true;
    bool statusVisible = false;
    bool lobbyCreated = false;
    QString m_error;
    QTimer statusTimer;
    quint64 m_ping = 0;
    PlayersList *m_playersList;
    int m_team = 3;
    int m_players_lobby_count = 1;
    QStringList m_TeamSelect;
};
