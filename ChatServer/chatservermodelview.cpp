#include "chatservermodelview.h"


//! @brief Конструктор класса
ChatServerModelView::ChatServerModelView()
{
    const QString serverName = "ChatWebSocketServer";

    m_server = new QWebSocketServer(serverName, QWebSocketServer::NonSecureMode);
    connect(m_server, &QWebSocketServer::newConnection, this, &ChatServerModelView::onClientConnected);

    m_client = nullptr;
    QString errorMessage;
}

//! @brief Получение адреса сервера
const QString ChatServerModelView::getSenderName()
{
    return m_server->serverAddress().toString() + ":" + QString::number(m_server->serverPort());
}

/*! @brief Запуск сервера
 *
 *  @param t_serverPort Порт запускаемого сервера
 *  @param t_errorMessage Ошибка, которую может вернуть метод
 *
 *  @return void
*/
void ChatServerModelView::startServer(const QString t_serverPort, QString &t_errorMessage)
{
    bool converted;
    int serverPort = t_serverPort.toInt(&converted);
    if (!converted) {
        t_errorMessage = "Неверный формат порта сервера";
        return;
    }

    qDebug() << m_server->isListening();
    if (m_server->isListening()) {
        m_server->close();
    }
    if (m_client) {
        m_client->close();
    }

    if (m_server->listen(QHostAddress::Any, serverPort)) {
        t_errorMessage = m_server->errorString();
        return;
    }
}

/*! @brief Отправка сообщений клиенту
 *
 *  @param t_messageText Текст сообщения, которое будет оптравлено
 *  @param t_errorMessage Ошибка, которую может вернуть метод
 *
 *  @return void
*/
void ChatServerModelView::sendMessageToClient(QString t_messageText, QString &t_errorMessage)
{
    if (t_messageText.isEmpty()) {
        t_errorMessage = "Сообщение не может быть пустым";
        return;
    }

    if (m_client == nullptr) {
        t_errorMessage = "Клиент не подключен";
        return;
    }

    m_client->sendTextMessage(t_messageText);

    m_dbAccessor.addMessageToDatabase(getSenderName(), t_messageText, t_errorMessage);
    if (!t_errorMessage.isEmpty()) {
        return;
    }
}

/*! @brief Событие при подключении клиента к серверу
 *
 *  @return void
*/
void ChatServerModelView::onClientConnected()
{
    QWebSocket *connectedSocket = m_server->nextPendingConnection();
    if (m_client) {
        m_client->close();
    }
    m_client = connectedSocket;

    emit clientConnected();
    connect(connectedSocket, &QWebSocket::disconnected, this, &ChatServerModelView::onClientDisconnected);
    connect(connectedSocket, &QWebSocket::textMessageReceived, this, &ChatServerModelView::onMessageReceived);
}

/*! @brief Событие при отключении клиента от сервера
 *
 *  @return void
*/
void ChatServerModelView::onClientDisconnected()
{
    m_client = nullptr;
    emit clientDisconnected();
}

/*! @brief Событие при получении сообщения от клиента
 *
 *  @param t_message Сообщение, которое отправил клиент
 *
 *  @return void
*/
void ChatServerModelView::onMessageReceived(const QString t_message)
{
    QWebSocket *socket = qobject_cast<QWebSocket*>(QObject::sender());
    socket->sendBinaryMessage(QByteArray("received"));

    m_server->serverAddress().toString() + ":" + QString::number(m_server->serverPort());
    QString senderName = socket->peerAddress().toString() + ":" + QString::number(socket->peerPort());

    QString errorMessage;
    m_dbAccessor.addMessageToDatabase(senderName, t_message, errorMessage);

    emit messsageReceived(senderName, t_message);
}
