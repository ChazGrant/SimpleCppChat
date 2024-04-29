#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWebSocketServer>
#include <QWebSocket>
#include <QMessageBox>
#include <QDebug>

#include "usernotifier.h"
#include "chatservermodelview.h"


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

private:
    Ui::MainWindow *ui;

    void addMessageToChat(const QString t_senderName, const QString t_textMessage);
    void updateEvents(const QString t_newEvent);

    ChatServerModelView *m_chatServer = nullptr;
    void setServerSignals();
};
#endif // MAINWINDOW_H
