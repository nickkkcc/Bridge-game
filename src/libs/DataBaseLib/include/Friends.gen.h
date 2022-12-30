/************************************************************************************************
** File created by QxEntityEditor 1.2.6 (2022/12/29 04:05) : please, do NOT modify this file ! **
************************************************************************************************/

#ifndef _BRIDGESERVER_FRIENDS_H_
#define _BRIDGESERVER_FRIENDS_H_

#include "../include/QuestionsType.gen.h"

class Users;

class BRIDGESERVER_EXPORT Friends
{

   QX_REGISTER_FRIEND_CLASS(Friends)

public:

   typedef QSharedPointer<Users> type_Users_id;

protected:

   long m_Friends_id;
   QString m_Login;
   QUuid m_Alias;
   QString m_Password;
   QuestionsType::enum_QuestionsType m_Question_type;
   QString m_Question_anwer;
   double m_Score;
   type_Users_id m_Users_id;

public:

   Friends();
   Friends(const long & id);
   virtual ~Friends();

   long getFriends_id() const;
   QString getLogin() const;
   QUuid getAlias() const;
   QString getPassword() const;
   QuestionsType::enum_QuestionsType getQuestion_type() const;
   QString getQuestion_anwer() const;
   double getScore() const;
   type_Users_id getUsers_id() const;

   void setFriends_id(const long & val);
   void setLogin(const QString & val);
   void setAlias(const QUuid & val);
   void setPassword(const QString & val);
   void setQuestion_type(const QuestionsType::enum_QuestionsType & val);
   void setQuestion_anwer(const QString & val);
   void setScore(const double & val);
   void setUsers_id(const type_Users_id & val);

   type_Users_id getUsers_id(bool bLoadFromDatabase, const QString & sAppendRelations = QString(), QSqlDatabase * pDatabase = NULL, QSqlError * pDaoError = NULL);

public:

   static QString relation_Users_id(bool key = false) { Q_UNUSED(key); return "Users_id"; }

public:

   static QString column_Friends_id(bool key = false) { Q_UNUSED(key); return "Friends_id"; }
   static QString column_Login(bool key = false) { Q_UNUSED(key); return "Login"; }
   static QString column_Alias(bool key = false) { Q_UNUSED(key); return "Alias"; }
   static QString column_Password(bool key = false) { Q_UNUSED(key); return "Password"; }
   static QString column_Question_type(bool key = false) { Q_UNUSED(key); return "Question_type"; }
   static QString column_Question_anwer(bool key = false) { Q_UNUSED(key); return "Question_anwer"; }
   static QString column_Score(bool key = false) { Q_UNUSED(key); return "Score"; }

public:

   static QString table_name(bool key = false) { return (key ? QString("Friends") : QString("t_Friends")); }

};

typedef std::shared_ptr<Friends> Friends_ptr;
typedef qx::QxCollection<long, Friends_ptr> list_of_Friends;
typedef std::shared_ptr<list_of_Friends> list_of_Friends_ptr;

QX_REGISTER_COMPLEX_CLASS_NAME_HPP_BRIDGESERVER(Friends, qx::trait::no_base_class_defined, 0, Friends)

#include "../include/Users.gen.h"

#endif // _BRIDGESERVER_FRIENDS_H_
