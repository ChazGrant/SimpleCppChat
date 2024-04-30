#ifndef CHATSERVERMODELVIEW_H
#define CHATSERVERMODELVIEW_H

#include <QObject>

#include <QWebSocketServer>
#include <QWebSocket>

#include "databaseaccessor.h"


class ChatServerModelView : public QObject
{
    Q_OBJECT
public:
    ChatServerModelView();

    const QString getSenderName();
    const QString getLastSentMessage();

    void startServer(const QString t_serverPort, QString &t_errorMessage);
    void sendMessageToClient(QString t_messageText, QString &t_errorMessage);
signals:
    void clientDisconnected();
    void clientConnected();
    void messsageReceived(const QString, const QString);
    void clientReceivedMessage(const QString t_chatClientName, const QString t_lastSentMessage);
private slots:
    void onClientConnected();
    void onClientDisconnected();
    void onMessageReceived(const QString t_message);
    void onClientMessageReceived(const QByteArray &t_message);

private:
    //! Указатель на сокет клиента
    QWebSocket *m_client;
    //! Указатель на сокет запущенного сервера
    QWebSocketServer *m_server;
    //! Указатель на класс обработчика базы данных
    DatabaseAccessor m_dbAccessor = DatabaseAccessor::getInstance();

    //! Получено ли сообщение клиентом
    bool m_messageReceivedByClient = false;
    //! Последнее отправленное сообщение
    QString m_lastSentMessage;

    void addMessageToDatabase(const QString t_senderName, const QString t_messageText, QString &t_errorMessage);
};

#endif // CHATSERVERMODELVIEW_H
