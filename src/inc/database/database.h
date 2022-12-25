#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <bridgeServer_all_include.gen.h>
#include <bridgeServer_export.gen.h>
#include <bridgeServer_precompiled_header.gen.h>
#include <QxOrm.h>

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QString dataBaseName,
                      QString hostName,
                      int port,
                      QString userName,
                      QString password,
                      QString driverName = "QPSQL",
                      QObject *parent = nullptr);
    bool writeUserToBase(QString login, QString password,
                    QString questionType, QString answer, double score);
    bool readUserFromBase(QString login);
    const QString getUserLogin() const;
    const QString getuserPassword() const;
    const QString getUserQuestionType() const;
    const QString getUseranswer() const;
    int getUserScore();
    ~DataBase();

  private:
    QSharedPointer<Users> userPtr;
    QVector<Users> usersList;
  signals:
};

#endif // DATABASE_H
