/************************************************************************************************
** File created by QxEntityEditor 1.2.6 (2022/12/29 06:21) : please, do NOT modify this file ! **
************************************************************************************************/

#include "../include/bridgeServer_precompiled_header.gen.h"

#include "../include/History.gen.h"
#include "../include/Users.gen.h"

#include <QxOrm_Impl.h>

QX_REGISTER_COMPLEX_CLASS_NAME_CPP_BRIDGESERVER(History, History)

namespace qx {

template <>
void register_class(QxClass<History> & t)
{
   qx::IxDataMember * pData = NULL; Q_UNUSED(pData);
   qx::IxSqlRelation * pRelation = NULL; Q_UNUSED(pRelation);
   qx::IxFunction * pFct = NULL; Q_UNUSED(pFct);
   qx::IxValidator * pValidator = NULL; Q_UNUSED(pValidator);

   t.setName("t_History");

   pData = t.id(& History::m_History_id, "History_id", 0);

   pData = t.data(& History::m_Game_start, "Game_start", 0, true, true);
   pData = t.data(& History::m_Game_end, "Game_end", 0, true, true);
   pData = t.data(& History::m_Owner_login, "Owner_login", 0, true, true);
   pData->setIsUnique(true);
   pData = t.data(& History::m_Owner_alias, "Owner_alias", 0, true, true);
   pData->setIsUnique(true);
   pData = t.data(& History::m_Winner_team, "Winner_team", 0, true, true);
   pData = t.data(& History::m_Player_N_login, "Player_N_login", 0, true, true);
   pData = t.data(& History::m_Player_N_alias, "Player_N_alias", 0, true, true);
   pData = t.data(& History::m_Player_S_login, "Player_S_login", 0, true, true);
   pData = t.data(& History::m_Player_S_alias, "Player_S_alias", 0, true, true);
   pData = t.data(& History::m_Player_E_login, "Player_E_login", 0, true, true);
   pData = t.data(& History::m_Player_E_alias, "Player_E_alias", 0, true, true);
   pData = t.data(& History::m_Player_W_login, "Player_W_login", 0, true, true);
   pData = t.data(& History::m_Player_W_alias, "Player_W_alias", 0, true, true);
   pData = t.data(& History::m_Total_score_NS, "Total_score_NS", 0, true, true);
   pData = t.data(& History::m_Total_score_EW, "Total_score_EW", 0, true, true);

   pRelation = t.relationManyToMany(& History::m_list_of_Users, "list_of_Users", "t_qxee_History_Users", "History_id", "Users_id", 0);

   qx::QxValidatorX<History> * pAllValidator = t.getAllValidator(); Q_UNUSED(pAllValidator);
   pAllValidator->add_NotNull("Game_start");
   pAllValidator->add_NotNull("Game_end");
   pAllValidator->add_NotNull("Owner_login");
   pAllValidator->add_NotNull("Owner_alias");
   pAllValidator->add_NotNull("Winner_team");
   pAllValidator->add_NotNull("Player_N_login");
   pAllValidator->add_NotNull("Player_N_alias");
   pAllValidator->add_NotNull("Player_S_login");
   pAllValidator->add_NotNull("Player_S_alias");
   pAllValidator->add_NotNull("Player_E_login");
   pAllValidator->add_NotNull("Player_E_alias");
   pAllValidator->add_NotNull("Player_W_login");
   pAllValidator->add_NotNull("Player_W_alias");
   pAllValidator->add_NotNull("Total_score_NS");
   pAllValidator->add_NotNull("Total_score_EW");
}

} // namespace qx

History::History() : m_History_id(0), m_Winner_team(0), m_Total_score_NS(0), m_Total_score_EW(0) { ; }

History::History(const long & id) : m_History_id(id), m_Winner_team(0), m_Total_score_NS(0), m_Total_score_EW(0) { ; }

History::~History() { ; }

long History::getHistory_id() const { return m_History_id; }

QDateTime History::getGame_start() const { return m_Game_start; }

QDateTime History::getGame_end() const { return m_Game_end; }

QString History::getOwner_login() const { return m_Owner_login; }

QUuid History::getOwner_alias() const { return m_Owner_alias; }

unsigned short History::getWinner_team() const { return m_Winner_team; }

QString History::getPlayer_N_login() const { return m_Player_N_login; }

QUuid History::getPlayer_N_alias() const { return m_Player_N_alias; }

QString History::getPlayer_S_login() const { return m_Player_S_login; }

QUuid History::getPlayer_S_alias() const { return m_Player_S_alias; }

QString History::getPlayer_E_login() const { return m_Player_E_login; }

QUuid History::getPlayer_E_alias() const { return m_Player_E_alias; }

QString History::getPlayer_W_login() const { return m_Player_W_login; }

QUuid History::getPlayer_W_alias() const { return m_Player_W_alias; }

qint64 History::getTotal_score_NS() const { return m_Total_score_NS; }

qint64 History::getTotal_score_EW() const { return m_Total_score_EW; }

History::type_list_of_Users History::getlist_of_Users() const { return m_list_of_Users; }

History::type_list_of_Users & History::list_of_Users() { return m_list_of_Users; }

const History::type_list_of_Users & History::list_of_Users() const { return m_list_of_Users; }

void History::setHistory_id(const long & val) { m_History_id = val; }

void History::setGame_start(const QDateTime & val) { m_Game_start = val; }

void History::setGame_end(const QDateTime & val) { m_Game_end = val; }

void History::setOwner_login(const QString & val) { m_Owner_login = val; }

void History::setOwner_alias(const QUuid & val) { m_Owner_alias = val; }

void History::setWinner_team(const unsigned short & val) { m_Winner_team = val; }

void History::setPlayer_N_login(const QString & val) { m_Player_N_login = val; }

void History::setPlayer_N_alias(const QUuid & val) { m_Player_N_alias = val; }

void History::setPlayer_S_login(const QString & val) { m_Player_S_login = val; }

void History::setPlayer_S_alias(const QUuid & val) { m_Player_S_alias = val; }

void History::setPlayer_E_login(const QString & val) { m_Player_E_login = val; }

void History::setPlayer_E_alias(const QUuid & val) { m_Player_E_alias = val; }

void History::setPlayer_W_login(const QString & val) { m_Player_W_login = val; }

void History::setPlayer_W_alias(const QUuid & val) { m_Player_W_alias = val; }

void History::setTotal_score_NS(const qint64 & val) { m_Total_score_NS = val; }

void History::setTotal_score_EW(const qint64 & val) { m_Total_score_EW = val; }

void History::setlist_of_Users(const History::type_list_of_Users & val) { m_list_of_Users = val; }

History::type_list_of_Users History::getlist_of_Users(bool bLoadFromDatabase, const QString & sAppendRelations /* = QString() */, QSqlDatabase * pDatabase /* = NULL */, QSqlError * pDaoError /* = NULL */)
{
   if (pDaoError) { (* pDaoError) = QSqlError(); }
   if (! bLoadFromDatabase) { return getlist_of_Users(); }
   QString sRelation = "{History_id} | list_of_Users";
   if (! sAppendRelations.isEmpty() && ! sAppendRelations.startsWith("->") && ! sAppendRelations.startsWith(">>")) { sRelation += "->" + sAppendRelations; }
   else if (! sAppendRelations.isEmpty()) { sRelation += sAppendRelations; }
   History tmp;
   tmp.m_History_id = this->m_History_id;
   QSqlError daoError = qx::dao::fetch_by_id_with_relation(sRelation, tmp, pDatabase);
   if (! daoError.isValid()) { this->m_list_of_Users = tmp.m_list_of_Users; }
   if (pDaoError) { (* pDaoError) = daoError; }
   return m_list_of_Users;
}

History::type_list_of_Users & History::list_of_Users(bool bLoadFromDatabase, const QString & sAppendRelations /* = QString() */, QSqlDatabase * pDatabase /* = NULL */, QSqlError * pDaoError /* = NULL */)
{
   if (pDaoError) { (* pDaoError) = QSqlError(); }
   if (! bLoadFromDatabase) { return list_of_Users(); }
   QString sRelation = "{History_id} | list_of_Users";
   if (! sAppendRelations.isEmpty() && ! sAppendRelations.startsWith("->") && ! sAppendRelations.startsWith(">>")) { sRelation += "->" + sAppendRelations; }
   else if (! sAppendRelations.isEmpty()) { sRelation += sAppendRelations; }
   History tmp;
   tmp.m_History_id = this->m_History_id;
   QSqlError daoError = qx::dao::fetch_by_id_with_relation(sRelation, tmp, pDatabase);
   if (! daoError.isValid()) { this->m_list_of_Users = tmp.m_list_of_Users; }
   if (pDaoError) { (* pDaoError) = daoError; }
   return m_list_of_Users;
}

