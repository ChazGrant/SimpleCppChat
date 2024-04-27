#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->startServerButton, &QPushButton::clicked, this, &MainWindow::startServer);
    connect(ui->sendMessagePushButton, &QPushButton::clicked, this, &MainWindow::sendMessageToClient);

    ui->clientOnlineLabel->setText("Клиент не подключен");

    m_dbAccessor = DatabaseAccessor::getInstance();

    const QString serverName = "ChatWebSocketServer";
    m_webServer = new QWebSocketServer(serverName, QWebSocketServer::NonSecureMode);
    connect(m_webServer, &QWebSocketServer::newConnection, this, &MainWindow::onSocketConnected);
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
        m_clientSocket->close();
        m_webServer->close();
    }

    bool serverStarted = m_webServer->listen(QHostAddress::Any, serverPort);
    if (serverStarted) {
        UserNotifier::showMessage("Сервер запущен", QMessageBox::Icon::Information);
    } else {
        UserNotifier::showMessage(m_webServer->errorString(), QMessageBox::Icon::Critical);
    }
}

void MainWindow::sendMessageToClient()
{
    QString messageText = ui->chatMessageTextEdit->toPlainText();
    if (messageText.isEmpty()) {
        return UserNotifier::showMessage("Сообщение не может быть пустым", QMessageBox::Icon::Critical);
    }

    if (m_clientSocket == nullptr) {
        return UserNotifier::showMessage("Клиент не подключен", QMessageBox::Icon::Critical);
    }

    addMessageToChat(m_webServer->serverAddress().toString(), m_webServer->serverPort(), messageText);
    m_clientSocket->sendTextMessage(messageText);

    bool inserted = m_dbAccessor->insertNewMessage(m_webServer->serverAddress().toString(),
                                                   m_webServer->serverPort(),
                                                   messageText);
    if (!inserted) {
        UserNotifier::showMessage("Ошибка при занесении информации в базу: " + m_dbAccessor->getLastError(),
                                  QMessageBox::Icon::Critical);
    }
}

void MainWindow::onSocketConnected()
{
    qDebug() << "Socket connected";
    QWebSocket *connectedSocket = m_webServer->nextPendingConnection();
    if (m_clientSocket) {
        m_clientSocket->close();
    }
    m_clientSocket = connectedSocket;

    connect(connectedSocket, &QWebSocket::disconnected, this, &MainWindow::onSocketDisconnected);
    connect(connectedSocket, &QWebSocket::textMessageReceived, this, &MainWindow::onMessageReceived);

    updateEvents("Клиент подключился");
    ui->clientOnlineLabel->setText("Клиент подключен");
}

void MainWindow::onSocketDisconnected()
{
    m_clientSocket = nullptr;

    updateEvents("Клиент отключился");
    ui->clientOnlineLabel->setText("Клиент отключён");
}

void MainWindow::onMessageReceived(const QString t_message)
{
    QWebSocket *socket = qobject_cast<QWebSocket*>(QObject::sender());
    addMessageToChat(socket->peerAddress().toString(), socket->peerPort(), t_message);

    bool inserted = m_dbAccessor->insertNewMessage(socket->peerAddress().toString(), socket->peerPort(), t_message);
    if (!inserted) {
        UserNotifier::showMessage("Ошибка при занесении информации в базу: " + m_dbAccessor->getLastError(),
                                  QMessageBox::Icon::Critical);
    }
}

void MainWindow::addMessageToChat(const QString t_senderAddress, const int t_senderPort, const QString t_textMessage)
{
    QString chatMessages = ui->chatTextBrowser->toPlainText();
    ui->chatTextBrowser->setText(chatMessages +
                                QString("\nНовое сообщение от %1:%2\n%3").arg(
                                    t_senderAddress, QString::number(t_senderPort), t_textMessage));
}

void MainWindow::updateEvents(const QString t_newEvent)
{
    QString shownEvents = ui->eventsTextBrowser->toPlainText();
    ui->eventsTextBrowser->setText(shownEvents + "\n" + t_newEvent);
}
