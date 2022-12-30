/************************************************************************************************
** File created by QxEntityEditor 1.2.6 (2022/12/29 06:21) : please, do NOT modify this file ! **
************************************************************************************************/

#ifndef _BRIDGESERVER_HISTORY_H_
#define _BRIDGESERVER_HISTORY_H_

class Users;

class BRIDGESERVER_EXPORT History
{

   QX_REGISTER_FRIEND_CLASS(History)

public:

   typedef QVector<QSharedPointer<Users> > type_list_of_Users;

protected:

   long m_History_id;
   QDateTime m_Game_start;
   QDateTime m_Game_end;
   QString m_Owner_login;
   QUuid m_Owner_alias;
   unsigned short m_Winner_team;
   QString m_Player_N_login;
   QUuid m_Player_N_alias;
   QString m_Player_S_login;
   QUuid m_Player_S_alias;
   QString m_Player_E_login;
   QUuid m_Player_E_alias;
   QString m_Player_W_login;
   QUuid m_Player_W_alias;
   qint64 m_Total_score_NS;
   qint64 m_Total_score_EW;
   type_list_of_Users m_list_of_Users;

public:

   History();
   History(const long & id);
   virtual ~History();

   long getHistory_id() const;
   QDateTime getGame_start() const;
   QDateTime getGame_end() const;
   QString getOwner_login() const;
   QUuid getOwner_alias() const;
   unsigned short getWinner_team() const;
   QString getPlayer_N_login() const;
   QUuid getPlayer_N_alias() const;
   QString getPlayer_S_login() const;
   QUuid getPlayer_S_alias() const;
   QString getPlayer_E_login() const;
   QUuid getPlayer_E_alias() const;
   QString getPlayer_W_login() const;
   QUuid getPlayer_W_alias() const;
   qint64 getTotal_score_NS() const;
   qint64 getTotal_score_EW() const;
   type_list_of_Users getlist_of_Users() const;
   type_list_of_Users & list_of_Users();
   const type_list_of_Users & list_of_Users() const;

   void setHistory_id(const long & val);
   void setGame_start(const QDateTime & val);
   void setGame_end(const QDateTime & val);
   void setOwner_login(const QString & val);
   void setOwner_alias(const QUuid & val);
   void setWinner_team(const unsigned short & val);
   void setPlayer_N_login(const QString & val);
   void setPlayer_N_alias(const QUuid & val);
   void setPlayer_S_login(const QString & val);
   void setPlayer_S_alias(const QUuid & val);
   void setPlayer_E_login(const QString & val);
   void setPlayer_E_alias(const QUuid & val);
   void setPlayer_W_login(const QString & val);
   void setPlayer_W_alias(const QUuid & val);
   void setTotal_score_NS(const qint64 & val);
   void setTotal_score_EW(const qint64 & val);
   void setlist_of_Users(const type_list_of_Users & val);

   type_list_of_Users getlist_of_Users(bool bLoadFromDatabase, const QString & sAppendRelations = QString(), QSqlDatabase * pDatabase = NULL, QSqlError * pDaoError = NULL);
   type_list_of_Users & list_of_Users(bool bLoadFromDatabase, const QString & sAppendRelations = QString(), QSqlDatabase * pDatabase = NULL, QSqlError * pDaoError = NULL);

public:

   static QString relation_list_of_Users(bool key = false) { Q_UNUSED(key); return "list_of_Users"; }

public:

   static QString column_History_id(bool key = false) { Q_UNUSED(key); return "History_id"; }
   static QString column_Game_start(bool key = false) { Q_UNUSED(key); return "Game_start"; }
   static QString column_Game_end(bool key = false) { Q_UNUSED(key); return "Game_end"; }
   static QString column_Owner_login(bool key = false) { Q_UNUSED(key); return "Owner_login"; }
   static QString column_Owner_alias(bool key = false) { Q_UNUSED(key); return "Owner_alias"; }
   static QString column_Winner_team(bool key = false) { Q_UNUSED(key); return "Winner_team"; }
   static QString column_Player_N_login(bool key = false) { Q_UNUSED(key); return "Player_N_login"; }
   static QString column_Player_N_alias(bool key = false) { Q_UNUSED(key); return "Player_N_alias"; }
   static QString column_Player_S_login(bool key = false) { Q_UNUSED(key); return "Player_S_login"; }
   static QString column_Player_S_alias(bool key = false) { Q_UNUSED(key); return "Player_S_alias"; }
   static QString column_Player_E_login(bool key = false) { Q_UNUSED(key); return "Player_E_login"; }
   static QString column_Player_E_alias(bool key = false) { Q_UNUSED(key); return "Player_E_alias"; }
   static QString column_Player_W_login(bool key = false) { Q_UNUSED(key); return "Player_W_login"; }
   static QString column_Player_W_alias(bool key = false) { Q_UNUSED(key); return "Player_W_alias"; }
   static QString column_Total_score_NS(bool key = false) { Q_UNUSED(key); return "Total_score_NS"; }
   static QString column_Total_score_EW(bool key = false) { Q_UNUSED(key); return "Total_score_EW"; }

public:

   static QString table_name(bool key = false) { return (key ? QString("History") : QString("t_History")); }

};

typedef std::shared_ptr<History> History_ptr;
typedef qx::QxCollection<long, History_ptr> list_of_History;
typedef std::shared_ptr<list_of_History> list_of_History_ptr;

QX_REGISTER_COMPLEX_CLASS_NAME_HPP_BRIDGESERVER(History, qx::trait::no_base_class_defined, 0, History)

#include "../include/Users.gen.h"

#endif // _BRIDGESERVER_HISTORY_H_
