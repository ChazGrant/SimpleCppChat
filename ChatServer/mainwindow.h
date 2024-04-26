#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWebSocketServer>
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
    void startServer();

    void onSocketConnected();
    void onSocketDisconnected();
    void onMessageReceived(const QString t_message);

private:
    Ui::MainWindow *ui;

    QWebSocketServer *m_webServer;
};
#endif // MAINWINDOW_H
