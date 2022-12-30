/************************************************************************************************
** File created by QxEntityEditor 1.2.6 (2022/12/29 03:00) : please, do NOT modify this file ! **
************************************************************************************************/

#include "../include/bridgeServer_precompiled_header.gen.h"

#include "../include/Users.gen.h"
#include "../include/History.gen.h"
#include "../include/Friends.gen.h"

#include <QxOrm_Impl.h>

QX_REGISTER_COMPLEX_CLASS_NAME_CPP_BRIDGESERVER(Users, Users)

namespace qx {

template <>
void register_class(QxClass<Users> & t)
{
   qx::IxDataMember * pData = NULL; Q_UNUSED(pData);
   qx::IxSqlRelation * pRelation = NULL; Q_UNUSED(pRelation);
   qx::IxFunction * pFct = NULL; Q_UNUSED(pFct);
   qx::IxValidator * pValidator = NULL; Q_UNUSED(pValidator);

   t.setName("t_Users");

   pData = t.id(& Users::m_Users_id, "Users_id", 0);

   pData = t.data(& Users::m_Login, "Login", 0, true, true);
   pData->setIsUnique(true);
   pData = t.data(& Users::m_Alias, "Alias", 0, true, true);
   pData = t.data(& Users::m_Password, "Password", 0, true, true);
   pData = t.data(& Users::m_Question_type, "Question_type", 0, true, true);
   pData = t.data(& Users::m_Question_answer, "Question_answer", 0, true, true);
   pData = t.data(& Users::m_Score, "Score", 0, true, true);

   pRelation = t.relationManyToMany(& Users::m_list_of_History, "list_of_History", "t_qxee_History_Users", "Users_id", "History_id", 0);
   pRelation = t.relationOneToMany(& Users::m_list_of_Friends, "list_of_Friends", "Users_id", 0);

   qx::QxValidatorX<Users> * pAllValidator = t.getAllValidator(); Q_UNUSED(pAllValidator);
   pAllValidator->add_NotNull("Login");
   pAllValidator->add_NotNull("Alias");
   pAllValidator->add_NotNull("Password");
   pAllValidator->add_NotNull("Question_type");
   pAllValidator->add_NotNull("Question_answer");
   pAllValidator->add_NotNull("Score");
}

} // namespace qx

Users::Users() : m_Users_id(0), m_Score(0) { ; }

Users::Users(const long & id) : m_Users_id(id), m_Score(0) { ; }

Users::~Users() { ; }

long Users::getUsers_id() const { return m_Users_id; }

QString Users::getLogin() const { return m_Login; }

QUuid Users::getAlias() const { return m_Alias; }

QString Users::getPassword() const { return m_Password; }

QuestionsType::enum_QuestionsType Users::getQuestion_type() const { return m_Question_type; }

QString Users::getQuestion_answer() const { return m_Question_answer; }

double Users::getScore() const { return m_Score; }

Users::type_list_of_History Users::getlist_of_History() const { return m_list_of_History; }

Users::type_list_of_History & Users::list_of_History() { return m_list_of_History; }

const Users::type_list_of_History & Users::list_of_History() const { return m_list_of_History; }

Users::type_list_of_Friends Users::getlist_of_Friends() const { return m_list_of_Friends; }

Users::type_list_of_Friends & Users::list_of_Friends() { return m_list_of_Friends; }

const Users::type_list_of_Friends & Users::list_of_Friends() const { return m_list_of_Friends; }

void Users::setUsers_id(const long & val) { m_Users_id = val; }

void Users::setLogin(const QString & val) { m_Login = val; }

void Users::setAlias(const QUuid & val) { m_Alias = val; }

void Users::setPassword(const QString & val) { m_Password = val; }

void Users::setQuestion_type(const QuestionsType::enum_QuestionsType & val) { m_Question_type = val; }

void Users::setQuestion_answer(const QString & val) { m_Question_answer = val; }

void Users::setScore(const double & val) { m_Score = val; }

void Users::setlist_of_History(const Users::type_list_of_History & val) { m_list_of_History = val; }

void Users::setlist_of_Friends(const Users::type_list_of_Friends & val) { m_list_of_Friends = val; }

Users::type_list_of_History Users::getlist_of_History(bool bLoadFromDatabase, const QString & sAppendRelations /* = QString() */, QSqlDatabase * pDatabase /* = NULL */, QSqlError * pDaoError /* = NULL */)
{
   if (pDaoError) { (* pDaoError) = QSqlError(); }
   if (! bLoadFromDatabase) { return getlist_of_History(); }
   QString sRelation = "{Users_id} | list_of_History";
   if (! sAppendRelations.isEmpty() && ! sAppendRelations.startsWith("->") && ! sAppendRelations.startsWith(">>")) { sRelation += "->" + sAppendRelations; }
   else if (! sAppendRelations.isEmpty()) { sRelation += sAppendRelations; }
   Users tmp;
   tmp.m_Users_id = this->m_Users_id;
   QSqlError daoError = qx::dao::fetch_by_id_with_relation(sRelation, tmp, pDatabase);
   if (! daoError.isValid()) { this->m_list_of_History = tmp.m_list_of_History; }
   if (pDaoError) { (* pDaoError) = daoError; }
   return m_list_of_History;
}

Users::type_list_of_History & Users::list_of_History(bool bLoadFromDatabase, const QString & sAppendRelations /* = QString() */, QSqlDatabase * pDatabase /* = NULL */, QSqlError * pDaoError /* = NULL */)
{
   if (pDaoError) { (* pDaoError) = QSqlError(); }
   if (! bLoadFromDatabase) { return list_of_History(); }
   QString sRelation = "{Users_id} | list_of_History";
   if (! sAppendRelations.isEmpty() && ! sAppendRelations.startsWith("->") && ! sAppendRelations.startsWith(">>")) { sRelation += "->" + sAppendRelations; }
   else if (! sAppendRelations.isEmpty()) { sRelation += sAppendRelations; }
   Users tmp;
   tmp.m_Users_id = this->m_Users_id;
   QSqlError daoError = qx::dao::fetch_by_id_with_relation(sRelation, tmp, pDatabase);
   if (! daoError.isValid()) { this->m_list_of_History = tmp.m_list_of_History; }
   if (pDaoError) { (* pDaoError) = daoError; }
   return m_list_of_History;
}

Users::type_list_of_Friends Users::getlist_of_Friends(bool bLoadFromDatabase, const QString & sAppendRelations /* = QString() */, QSqlDatabase * pDatabase /* = NULL */, QSqlError * pDaoError /* = NULL */)
{
   if (pDaoError) { (* pDaoError) = QSqlError(); }
   if (! bLoadFromDatabase) { return getlist_of_Friends(); }
   QString sRelation = "{Users_id} | list_of_Friends";
   if (! sAppendRelations.isEmpty() && ! sAppendRelations.startsWith("->") && ! sAppendRelations.startsWith(">>")) { sRelation += "->" + sAppendRelations; }
   else if (! sAppendRelations.isEmpty()) { sRelation += sAppendRelations; }
   Users tmp;
   tmp.m_Users_id = this->m_Users_id;
   QSqlError daoError = qx::dao::fetch_by_id_with_relation(sRelation, tmp, pDatabase);
   if (! daoError.isValid()) { this->m_list_of_Friends = tmp.m_list_of_Friends; }
   if (pDaoError) { (* pDaoError) = daoError; }
   return m_list_of_Friends;
}

Users::type_list_of_Friends & Users::list_of_Friends(bool bLoadFromDatabase, const QString & sAppendRelations /* = QString() */, QSqlDatabase * pDatabase /* = NULL */, QSqlError * pDaoError /* = NULL */)
{
   if (pDaoError) { (* pDaoError) = QSqlError(); }
   if (! bLoadFromDatabase) { return list_of_Friends(); }
   QString sRelation = "{Users_id} | list_of_Friends";
   if (! sAppendRelations.isEmpty() && ! sAppendRelations.startsWith("->") && ! sAppendRelations.startsWith(">>")) { sRelation += "->" + sAppendRelations; }
   else if (! sAppendRelations.isEmpty()) { sRelation += sAppendRelations; }
   Users tmp;
   tmp.m_Users_id = this->m_Users_id;
   QSqlError daoError = qx::dao::fetch_by_id_with_relation(sRelation, tmp, pDatabase);
   if (! daoError.isValid()) { this->m_list_of_Friends = tmp.m_list_of_Friends; }
   if (pDaoError) { (* pDaoError) = daoError; }
   return m_list_of_Friends;
}

