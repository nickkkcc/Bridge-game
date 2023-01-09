#pragma once

#include <QObject>

enum FindPopupState
{
    SET_JOIN,
    UNSET_JOIN,
    RX_INVITING,
    TX_ACCEPT_INVITE,
    RX_ACCEPT_INVITE,
    RX_SELECT_TEAM,
    TX_SELECT_TEAM,
    RX_RESELECT_TEAM,
    RX_IN_LOBBY
};

class FindLobbyManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int selectedTeam READ selectedTeam WRITE setSelectedTeam NOTIFY selectedTeamChanged)
    Q_PROPERTY(int lobbyPlayerCont READ lobbyPlayerCont WRITE setLobbyPlayerCont NOTIFY lobbyPlayerContChanged)
    Q_PROPERTY(bool reject READ reject WRITE setReject NOTIFY rejectChanged)
    Q_PROPERTY(bool isOpen READ isOpen WRITE setIsOpen NOTIFY isOpenChanged)
    Q_PROPERTY(int stateInt READ stateInt WRITE setStateInt NOTIFY stateIntChanged)
    Q_PROPERTY(bool selfExit READ getSelfExit WRITE setSelfExit NOTIFY selfExitChanged)

  public:
    explicit FindLobbyManager(QObject *parent = nullptr);

  public:
    void updateStates(FindPopupState state, QString inviterId = "", QString inviterLobbyId = "", int nsCount = 0,
                      int ewCount = 0);
    Q_INVOKABLE QString headerText();
    Q_INVOKABLE QString selectTeamText();
    Q_INVOKABLE QString statusText();
    Q_INVOKABLE QString counPlayerText();

    Q_INVOKABLE bool listEnabled();
    Q_INVOKABLE bool acceptBtnEnabled();

    QString inviterLobbyId() const;

    void setInviter_NS_count(int newInviter_NS_count);

    int selectedTeam() const;
    void setSelectedTeam(int newSelectedTeam);

    int lobbyPlayerCont() const;
    void setLobbyPlayerCont(int newLobbyPlayerCont);

    bool reject() const;
    void setReject(bool newReject);

    bool isOpen() const;
    void setIsOpen(bool newIsOpen);

    int stateInt() const;
    void setStateInt(int newStateInt);

    bool getSelfExit() const;
    void setSelfExit(bool newSelfExit);

  signals:

    void selectedTeamChanged();

    void lobbyPlayerContChanged();

    void rejectChanged();

    void isOpenChanged();

    void stateIntChanged();

    void selfExitChanged();

  private:
    QString m_headerText;
    QString m_selectTeamText;
    QString m_statusText;
    QString m_counPlayerText;

    QString m_inviterId;
    QString m_inviterLobbyId;

    bool m_listEnabled;
    bool m_acceptBtnEnabled;

    int m_selectedTeam;
    int m_inviter_NS_count;
    int m_inviter_EW_count;
    int m_lobbyPlayerCont = 0;

    FindPopupState state = UNSET_JOIN;
    bool m_reject;
    bool m_isOpen;
    int m_stateInt = 0;
    bool selfExit = false;
};
