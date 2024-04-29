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

    void startServer(const QString t_serverPort, QString &t_errorMessage);
    void sendMessageToClient(QString t_messageText, QString &t_errorMessage);
signals:
    void clientDisconnected();
    void clientConnected();
    void messsageReceived(const QString, const QString);
private slots:
    void onClientConnected();
    void onClientDisconnected();
    void onMessageReceived(const QString t_message);

private:
    //! Указатель на сокет клиента
    QWebSocket *m_client;
    //! Указатель на сокет запущенного сервера
    QWebSocketServer *m_server;
    //! Указатель на класс обработчика базы данных
    DatabaseAccessor m_dbAccessor = DatabaseAccessor::getInstance();

    void addMessageToDatabase(const QString t_senderName, const QString t_messageText, QString &t_errorMessage);
};

#endif // CHATSERVERMODELVIEW_H
