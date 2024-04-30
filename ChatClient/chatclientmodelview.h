#ifndef CHATCLIENTMODELVIEW_H
#define CHATCLIENTMODELVIEW_H

#include <QString>
#include <QStringList>

#include <QUrl>
#include <QWebSocket>


class ChatClientModelView : public QObject
{
    Q_OBJECT

public:
    ChatClientModelView();

    const QString getSenderName();
    const QString getLastSentMessage();

    void connectToServer(QString t_serverAddress, QString t_serverPort, QString &t_errorMessage);
    void disconnectFromServer(QString &t_errorMessage);
    void sendMessage(const QString t_messageText, QString &t_errorMessage);
private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketMessageReceived(const QString t_message);
    void onServerMessageReceived(const QByteArray &t_message);

signals:
    void chatSocketDisconnected();
    void chatSocketConnected();
    void chatSocketMessageReceived(QString t_chatClientName, const QString t_message);
    void serverReceivedMessage(const QString t_chatClientName, const QString t_lastSentMessage);

private:
    //! Адрес сервера к которому подключаемся
    QString m_serverAddress;
    //! Порт сервера к которому подключаемся
    QString m_serverPort;

    //! Установлено ли соединение
    bool m_connectionEstablished = false;
    //! Получено ли сообщение сервером
    bool m_messageReceivedByServer = false;
    //! Сокет, котороый подключается к серверу
    QWebSocket *m_chatSocket;

    //! Последнее отправленное сообщение
    QString m_lastSentMessage;

    void setSocketSignals();
};

#endif // CHATCLIENTMODELVIEW_H
