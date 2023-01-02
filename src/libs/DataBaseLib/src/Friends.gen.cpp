/************************************************************************************************
** File created by QxEntityEditor 1.2.6 (2023/01/01 22:53) : please, do NOT modify this file ! **
************************************************************************************************/

#include "../include/bridgeServer_precompiled_header.gen.h"

#include "../include/Friends.gen.h"
#include "../include/Users.gen.h"

#include <QxOrm_Impl.h>

QX_REGISTER_COMPLEX_CLASS_NAME_CPP_BRIDGESERVER(Friends, Friends)

namespace qx {

template <>
void register_class(QxClass<Friends> & t)
{
   qx::IxDataMember * pData = NULL; Q_UNUSED(pData);
   qx::IxSqlRelation * pRelation = NULL; Q_UNUSED(pRelation);
   qx::IxFunction * pFct = NULL; Q_UNUSED(pFct);
   qx::IxValidator * pValidator = NULL; Q_UNUSED(pValidator);

   t.setName("t_Friends");

   pData = t.id(& Friends::m_Friends_id, "Friends_id", 0);

   pData = t.data(& Friends::m_Login, "Login", 0, true, true);
   pData = t.data(& Friends::m_Alias, "Alias", 0, true, true);

   pRelation = t.relationManyToOne(& Friends::m_Users_id, "Users_id", 0);

   qx::QxValidatorX<Friends> * pAllValidator = t.getAllValidator(); Q_UNUSED(pAllValidator);
   pAllValidator->add_NotNull("Login");
   pAllValidator->add_NotNull("Alias");
}

} // namespace qx

Friends::Friends() : m_Friends_id(0) { ; }

Friends::Friends(const long & id) : m_Friends_id(id) { ; }

Friends::~Friends() { ; }

long Friends::getFriends_id() const { return m_Friends_id; }

QString Friends::getLogin() const { return m_Login; }

QUuid Friends::getAlias() const { return m_Alias; }

Friends::type_Users_id Friends::getUsers_id() const { return m_Users_id; }

void Friends::setFriends_id(const long & val) { m_Friends_id = val; }

void Friends::setLogin(const QString & val) { m_Login = val; }

void Friends::setAlias(const QUuid & val) { m_Alias = val; }

void Friends::setUsers_id(const Friends::type_Users_id & val) { m_Users_id = val; }

Friends::type_Users_id Friends::getUsers_id(bool bLoadFromDatabase, const QString & sAppendRelations /* = QString() */, QSqlDatabase * pDatabase /* = NULL */, QSqlError * pDaoError /* = NULL */)
{
   if (pDaoError) { (* pDaoError) = QSqlError(); }
   if (! bLoadFromDatabase) { return getUsers_id(); }
   QString sRelation = "{Friends_id} | Users_id";
   if (! sAppendRelations.isEmpty() && ! sAppendRelations.startsWith("->") && ! sAppendRelations.startsWith(">>")) { sRelation += "->" + sAppendRelations; }
   else if (! sAppendRelations.isEmpty()) { sRelation += sAppendRelations; }
   Friends tmp;
   tmp.m_Friends_id = this->m_Friends_id;
   QSqlError daoError = qx::dao::fetch_by_id_with_relation(sRelation, tmp, pDatabase);
   if (! daoError.isValid()) { this->m_Users_id = tmp.m_Users_id; }
   if (pDaoError) { (* pDaoError) = daoError; }
   return m_Users_id;
}

