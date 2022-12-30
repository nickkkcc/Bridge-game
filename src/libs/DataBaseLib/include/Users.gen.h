/************************************************************************************************
** File created by QxEntityEditor 1.2.6 (2022/12/29 03:00) : please, do NOT modify this file ! **
************************************************************************************************/

#ifndef _BRIDGESERVER_USERS_H_
#define _BRIDGESERVER_USERS_H_

#include "../include/QuestionsType.gen.h"

class History;
class Friends;

class BRIDGESERVER_EXPORT Users
{

   QX_REGISTER_FRIEND_CLASS(Users)

public:

   typedef QVector<QSharedPointer<History> > type_list_of_History;
   typedef QVector<QSharedPointer<Friends> > type_list_of_Friends;

protected:

   long m_Users_id;
   QString m_Login;
   QUuid m_Alias;
   QString m_Password;
   QuestionsType::enum_QuestionsType m_Question_type;
   QString m_Question_answer;
   double m_Score;
   type_list_of_History m_list_of_History;
   type_list_of_Friends m_list_of_Friends;

public:

   Users();
   Users(const long & id);
   virtual ~Users();

   long getUsers_id() const;
   QString getLogin() const;
   QUuid getAlias() const;
   QString getPassword() const;
   QuestionsType::enum_QuestionsType getQuestion_type() const;
   QString getQuestion_answer() const;
   double getScore() const;
   type_list_of_History getlist_of_History() const;
   type_list_of_History & list_of_History();
   const type_list_of_History & list_of_History() const;
   type_list_of_Friends getlist_of_Friends() const;
   type_list_of_Friends & list_of_Friends();
   const type_list_of_Friends & list_of_Friends() const;

   void setUsers_id(const long & val);
   void setLogin(const QString & val);
   void setAlias(const QUuid & val);
   void setPassword(const QString & val);
   void setQuestion_type(const QuestionsType::enum_QuestionsType & val);
   void setQuestion_answer(const QString & val);
   void setScore(const double & val);
   void setlist_of_History(const type_list_of_History & val);
   void setlist_of_Friends(const type_list_of_Friends & val);

   type_list_of_History getlist_of_History(bool bLoadFromDatabase, const QString & sAppendRelations = QString(), QSqlDatabase * pDatabase = NULL, QSqlError * pDaoError = NULL);
   type_list_of_History & list_of_History(bool bLoadFromDatabase, const QString & sAppendRelations = QString(), QSqlDatabase * pDatabase = NULL, QSqlError * pDaoError = NULL);
   type_list_of_Friends getlist_of_Friends(bool bLoadFromDatabase, const QString & sAppendRelations = QString(), QSqlDatabase * pDatabase = NULL, QSqlError * pDaoError = NULL);
   type_list_of_Friends & list_of_Friends(bool bLoadFromDatabase, const QString & sAppendRelations = QString(), QSqlDatabase * pDatabase = NULL, QSqlError * pDaoError = NULL);

public:

   static QString relation_list_of_History(bool key = false) { Q_UNUSED(key); return "list_of_History"; }
   static QString relation_list_of_Friends(bool key = false) { Q_UNUSED(key); return "list_of_Friends"; }

public:

   static QString column_Users_id(bool key = false) { Q_UNUSED(key); return "Users_id"; }
   static QString column_Login(bool key = false) { Q_UNUSED(key); return "Login"; }
   static QString column_Alias(bool key = false) { Q_UNUSED(key); return "Alias"; }
   static QString column_Password(bool key = false) { Q_UNUSED(key); return "Password"; }
   static QString column_Question_type(bool key = false) { Q_UNUSED(key); return "Question_type"; }
   static QString column_Question_answer(bool key = false) { Q_UNUSED(key); return "Question_answer"; }
   static QString column_Score(bool key = false) { Q_UNUSED(key); return "Score"; }

public:

   static QString table_name(bool key = false) { return (key ? QString("Users") : QString("t_Users")); }

};

typedef std::shared_ptr<Users> Users_ptr;
typedef qx::QxCollection<long, Users_ptr> list_of_Users;
typedef std::shared_ptr<list_of_Users> list_of_Users_ptr;

QX_REGISTER_COMPLEX_CLASS_NAME_HPP_BRIDGESERVER(Users, qx::trait::no_base_class_defined, 0, Users)

#include "../include/History.gen.h"
#include "../include/Friends.gen.h"

#endif // _BRIDGESERVER_USERS_H_
