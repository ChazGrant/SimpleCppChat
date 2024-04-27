#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWebSocket>
#include <QMessageBox>

#include "usernotifier.h"


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
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketMessageReceived(const QString t_message);

    void connectToServer();
    void disconnectFromServer();

    void setSocketSignals();

    void sendMessage();
    void addMessageToChat(const QString t_senderAddress, const int t_senderPort, const QString t_textMessage);
private:
    Ui::MainWindow *ui;

    bool m_serverConnected = false;
    QWebSocket *m_webSocket;
};
#endif // MAINWINDOW_H
