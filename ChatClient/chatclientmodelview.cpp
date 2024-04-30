#include "chatclientmodelview.h"


//! @brief Конструктор класса
ChatClientModelView::ChatClientModelView()
{

}

/*! @brief Получение имени отправителя
 *
 *  @return const QString
*/
const QString ChatClientModelView::getSenderName()
{
    return m_chatSocket->localAddress().toString() + ":" + QString::number(m_chatSocket->localPort());
}

/*! @brief Получение последнего отправленного сообщения
 *
 *  @return const QString
*/
const QString ChatClientModelView::getLastSentMessage()
{
    return m_lastSentMessage;
}

/*! @brief Подключение к серверу
 *
 *  @param t_serverAddress Адрес сервера
 *  @param t_serverPort Порт сервера
 *  @param t_errorMessage Ошибка, который может вернуть данный метод
 *
 *  @return void
*/
void ChatClientModelView::connectToServer(QString t_serverAddress, QString t_serverPort, QString &t_errorMessage)
{
    if (m_connectionEstablished) {
        t_errorMessage = "Соединение уже установлено";
        return;
    }

    bool converted;
    int serverPort = t_serverPort.toInt(&converted);
    if (t_serverAddress.isEmpty() || !converted) {
        t_errorMessage = "Адрес или порт сервера введены неверно";
        return;
    }

    QList<QString> serverAddressParts = t_serverAddress.split(".");
    if (serverAddressParts.size() != 4) {
        t_errorMessage = "Адрес сервера введён неверно";
        return;
    }
    for (int i = 0; i < serverAddressParts.size(); ++i) {
        serverAddressParts[i].toInt(&converted);
        if (!converted) {
            t_errorMessage = "Адрес сервера введён неверно";
            return;
        }
    }

    QUrl url;
    url.setScheme("ws");
    url.setHost(t_serverAddress);
    url.setPort(serverPort);

    m_chatSocket = new QWebSocket();
    setSocketSignals();
    m_chatSocket->open(url);
}

/*! @brief Отключение от сервера
 *
 *  @param &t_errorMessage Ошибка, который может вернуть данный метод
 *
 *  @return void
*/
void ChatClientModelView::disconnectFromServer(QString &t_errorMessage)
{
    qDebug() << m_chatSocket;
    if (!m_connectionEstablished || m_chatSocket == nullptr) {
        t_errorMessage = "Подключение не установлено";
        return;
    }

    m_chatSocket->close();
    return;
}

/*! @brief Отправка сообщения
 *
 *  @param t_messageText Текст сообщения
 *  @param &t_errorMessage Ошибка, который может вернуть данный метод
 *
 *  @return void
*/
void ChatClientModelView::sendMessage(const QString t_messageText, QString &t_errorMessage)
{
    if (!m_messageReceivedByServer && m_lastSentMessage.size()) {
        t_errorMessage = "Прошлое сообщение ещё не было обработано сервером";
        return;
    }
    if (!m_connectionEstablished) {
        t_errorMessage = "Вы не подключены к серверу";
        return;
    }
    if (t_messageText.isEmpty()) {
        t_errorMessage = "Сообщение не может быть пустым";
        return;
    }

    m_lastSentMessage = t_messageText;
    m_chatSocket->sendTextMessage(t_messageText);
    return;
}

/*! @brief Событие при подключении сокета к серверу
 *
 *  @return void
*/
void ChatClientModelView::onSocketConnected()
{
    m_connectionEstablished = true;
    emit chatSocketConnected();
}

/*! @brief Событие при отключении сокета от сервера
 *
 *  @return void
*/
void ChatClientModelView::onSocketDisconnected()
{
    m_connectionEstablished = false;
    m_chatSocket = nullptr;

    emit chatSocketDisconnected();
}

/*! @brief Событие при получении сообщения от сервера
 *
 *  @return void
*/
void ChatClientModelView::onSocketMessageReceived(const QString t_message)
{
    QWebSocket *senderSocket = qobject_cast<QWebSocket*>(QObject::sender());
    senderSocket->sendBinaryMessage(QByteArray("received"));

    QString socketName = senderSocket->peerAddress().toString() + ":" + QString::number(senderSocket->peerPort());

    emit chatSocketMessageReceived(socketName, t_message);
}

/*! @brief Событие срабатываемое когда сервер получил сообщение
 *
 *  @return void
*/
void ChatClientModelView::onServerMessageReceived(const QByteArray &t_message)
{
    const QString receivedMessage = QString(t_message);
    if (receivedMessage == "received") {
        m_messageReceivedByServer = true;
        emit serverReceivedMessage(getSenderName(), m_lastSentMessage);
        m_lastSentMessage = "";
    }
}

/*! @brief Установка сигналов для сокета
 *
 *  @return void
*/
void ChatClientModelView::setSocketSignals()
{
    disconnect(m_chatSocket, &QWebSocket::connected, this, &ChatClientModelView::onSocketConnected);
    disconnect(m_chatSocket, &QWebSocket::disconnected, this, &ChatClientModelView::onSocketDisconnected);
    disconnect(m_chatSocket, &QWebSocket::textMessageReceived, this, &ChatClientModelView::onSocketMessageReceived);
    disconnect(m_chatSocket, &QWebSocket::binaryMessageReceived, this, &ChatClientModelView::onServerMessageReceived);

    connect(m_chatSocket, &QWebSocket::connected, this, &ChatClientModelView::onSocketConnected);
    connect(m_chatSocket, &QWebSocket::disconnected, this, &ChatClientModelView::onSocketDisconnected);
    connect(m_chatSocket, &QWebSocket::textMessageReceived, this, &ChatClientModelView::onSocketMessageReceived);
    connect(m_chatSocket, &QWebSocket::binaryMessageReceived, this, &ChatClientModelView::onServerMessageReceived);
}
