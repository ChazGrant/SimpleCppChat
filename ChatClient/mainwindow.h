#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWebSocket>
#include <QMessageBox>

#include "usernotifier.h"
#include "chatclientmodelview.h"


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
    void connectToServer();
    void disconnectFromServer();

    void onChatSocketConnected();
    void onChatSocketDisconnected();

    void sendMessage();
    void updateChatMessages(const QString t_senderName, QString t_textMessage);
private:
    Ui::MainWindow *ui;

    //! Подключён ли клиент к серверу
    bool m_serverConnected = false;
    //! Указатель на класс ChatClientModelView
    ChatClientModelView *m_chatClient = nullptr;

    void setModelViewSignals();
};
#endif // MAINWINDOW_H
