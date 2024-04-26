#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->startServerButton, &QPushButton::clicked, this, &MainWindow::startServer);
    connect(m_webServer, &QWebSocketServer::newConnection, this, &MainWindow::onSocketConnected);

    const QString serverName = "ChatWebSocketServer";
    m_webServer = new QWebSocketServer(serverName, QWebSocketServer::SecureMode);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startServer()
{
    bool converted;
    int serverPort = ui->serverPortTextEdit->toPlainText().toInt(&converted);
    if (!converted) {
        return UserNotifier::showMessage("Неверный формат порта сервера", QMessageBox::Icon::Critical);
    }

     if (m_webServer->isListening()) {
         m_webServer->close();
     }
     m_webServer->listen(QHostAddress::Any, serverPort);
}

void MainWindow::onSocketConnected()
{
    QWebSocket *connectedSocket = m_webServer->nextPendingConnection();

    connect(connectedSocket, &QWebSocket::disconnected, this, &MainWindow::onSocketDisconnected);
    connect(connectedSocket, &QWebSocket::textMessageReceived, this, &MainWindow::onMessageReceived);

    QString shownText = ui->eventsTextBrowser->toPlainText();
    ui->eventsTextBrowser->setText(shownText + "\n" + "Клиент подключился");
}

void MainWindow::onSocketDisconnected()
{
    QString shownText = ui->eventsTextBrowser->toPlainText();

    ui->clientOnlineLabel->setText("Клиент отключён");
    ui->eventsTextBrowser->setText(shownText + "\n" + "Клиент отключился");
}

void MainWindow::onMessageReceived(const QString t_message)
{
    QWebSocket *socket = qobject_cast<QWebSocket*>(QObject::sender());
    QString chatMessages = ui->chatTextBrowser->toPlainText();
    ui->chatTextBrowser->setText(chatMessages + "\n" + "Новое сообщение от " + \
                                 socket->localAddress().toString() + ":\n" + t_message);
}
