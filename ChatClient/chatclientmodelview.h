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
    QString m_serverAddress;
    QString m_serverPort;
    QString m_lastSentMessage;

    bool m_connectionEstablished = false;
    bool m_messageReceivedByServer = false;
    QWebSocket *m_chatSocket;

    void setSocketSignals();
};

#endif // CHATCLIENTMODELVIEW_H
