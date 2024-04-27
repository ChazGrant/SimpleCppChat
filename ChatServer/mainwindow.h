#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWebSocketServer>
#include <QWebSocket>
#include <QMessageBox>
#include <QDebug>

#include "usernotifier.h"
#include "databaseaccessor.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startServer();

    void sendMessageToClient();

    void onSocketConnected();
    void onSocketDisconnected();
    void onMessageReceived(const QString t_message);

private:
    Ui::MainWindow *ui;

    void addMessageToChat(const QString t_senderAddress, const int t_senderPort, const QString t_textMessage);
    void updateEvents(const QString t_newEvent);

    QWebSocketServer *m_webServer;
    QWebSocket *m_clientSocket = nullptr;
    DatabaseAccessor *m_dbAccessor;
};
#endif // MAINWINDOW_H
