#ifndef DATABASEACCESSOR_H
#define DATABASEACCESSOR_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QVariant>

#include <QDebug>


class DatabaseAccessor
{
public:
    static DatabaseAccessor &getInstance();

    void addMessageToDatabase(const QString t_senderName, const QString t_messageText, QString &t_errorMessage);
    const QString getLastError();
private:
    DatabaseAccessor();

    void initTables();

    //! Запрос к базе данных
    QSqlQuery m_query;
    //! Объект базы данных
    QSqlDatabase m_db;
};

#endif // DATABASEACCESSOR_H
