#include "databaseaccessor.h"


/*! @brief Получение объекта базы данных
 *
 *  @return DatabaseAccessor
*/
DatabaseAccessor &DatabaseAccessor::getInstance()
{
    static DatabaseAccessor instance;
    return instance;
}

/*! @brief Добавление сообщения в базу данных
 *
 *  @param t_senderName Имя отправителя
 *  @param t_messageText Текст сообщения
 *  @param t_errorMessage Ошибка, которую может вернуть метод
 *
 *  @return void
*/
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

/*! @brief Получение последней ошибки базы данных
 *
 *  @return void
*/
const QString DatabaseAccessor::getLastError()
{
    return m_db.lastError().text();
}

//! @brief Констркутор класса
DatabaseAccessor::DatabaseAccessor()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_query = QSqlQuery(m_db);

    m_db.setDatabaseName("Chat.db");
    m_db.open();
    initTables();
}

/*! @brief Создание таблиц базы данных
 *
 *  @return void
*/
void DatabaseAccessor::initTables()
{
    m_query.exec("CREATE TABLE IF NOT EXISTS ChatMessages(messageId INTEGER PRIMARY KEY AUTOINCREMENT, senderName VARCHAR(20), messageText TEXT)");
    QString errorMessage;
}
