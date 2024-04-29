#include "databaseaccessor.h"

DatabaseAccessor &DatabaseAccessor::getInstance()
{
    static DatabaseAccessor instance;
    return instance;
}

void DatabaseAccessor::addMessageToDatabase(const QString t_senderName, const QString t_messageText, QString &t_errorMessage)
{
    m_query.prepare("INSERT INTO ChatMessages(senderName, messageText) VALUES(:senderName, :messageText)");

    m_query.bindValue(":senderName", t_senderName);
    m_query.bindValue(":messageText", t_messageText);

    bool executed = m_query.exec();
    if (!executed) {
        t_errorMessage = m_query.lastError().text();
        return;
    }
    m_db.commit();
}

const QString DatabaseAccessor::getLastError()
{
    return m_db.lastError().text();
}

DatabaseAccessor::DatabaseAccessor()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_query = QSqlQuery(m_db);

    m_db.setDatabaseName("Chat.db");
    m_db.open();
    initTables();
}

void DatabaseAccessor::initTables()
{
    m_query.exec("CREATE TABLE IF NOT EXISTS ChatMessages(messageId INTEGER PRIMARY KEY AUTOINCREMENT, senderName VARCHAR(20), messageText TEXT)");
    QString errorMessage;
}
