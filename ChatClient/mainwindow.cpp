#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->connectToServerButton, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(ui->disconnectFromServer, &QPushButton::clicked, this, &MainWindow::disconnectFromServer);
    connect(ui->sendMessageButton, &QPushButton::clicked, this, &MainWindow::sendMessage);

    ui->serverAddressTextEdit->setText("127.0.0.1");
    ui->serverPortTextEdit->setText("80");

    ui->serverConnectedLabel->setText("Подключение не установлено");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSocketConnected()
{
    m_serverConnected = true;
    ui->serverConnectedLabel->setText("Подключение установлено");
}

void MainWindow::onSocketDisconnected()
{
    qDebug() << "Disconnected";
    m_serverConnected = false;
    m_webSocket = nullptr;
    qDebug() << m_webSocket;

    ui->serverConnectedLabel->setText("Подключение не установлено");
}

void MainWindow::onSocketMessageReceived(const QString t_message)
{
    QWebSocket *server = qobject_cast<QWebSocket*>(QObject::sender());
    addMessageToChat(server->peerAddress().toString(), server->peerPort(), t_message);
}

void MainWindow::connectToServer()
{
    if (m_serverConnected) {
        return UserNotifier::showMessage("Сервер уже запущен", QMessageBox::Icon::Critical);
    }

    qDebug() << m_serverConnected;
    bool converted;
    QString serverAddress = ui->serverAddressTextEdit->toPlainText();
    int serverPort = ui->serverPortTextEdit->toPlainText().toInt(&converted);
    if (serverAddress.isEmpty() || !converted) {
        return UserNotifier::showMessage("Адрес или порт сервера введены неверно", QMessageBox::Icon::Critical);
    }

    QList<QString> serverAddressParts = serverAddress.split(".");
    if (serverAddressParts.size() != 4) {
        return UserNotifier::showMessage("Адрес сервера введён неверно", QMessageBox::Icon::Critical);
    }
    for (int i = 0; i < serverAddressParts.size(); ++i) {
        serverAddressParts[i].toInt(&converted);
        if (!converted) {
            return UserNotifier::showMessage("Адрес сервера введён неверно", QMessageBox::Icon::Critical);
        }
    }

    qDebug() << serverAddressParts;

    QUrl url;
    url.setScheme("ws");
    url.setHost(serverAddress);
    url.setPort(serverPort);

    m_webSocket = new QWebSocket();
    setSocketSignals();
    m_webSocket->open(url);
}

void MainWindow::disconnectFromServer()
{
    if (m_serverConnected) {
        m_webSocket->close();
        ui->serverConnectedLabel->setText("Подключение не установлено");
        m_serverConnected = false;
    } else {
        return UserNotifier::showMessage("Вы не подключены к серверу", QMessageBox::Icon::Critical);
    }
}

void MainWindow::setSocketSignals()
{
    disconnect(m_webSocket, &QWebSocket::connected, this, &MainWindow::onSocketConnected);
    disconnect(m_webSocket, &QWebSocket::disconnected, this, &MainWindow::onSocketDisconnected);
    disconnect(m_webSocket, &QWebSocket::textMessageReceived, this, &MainWindow::onSocketMessageReceived);

    connect(m_webSocket, &QWebSocket::connected, this, &MainWindow::onSocketConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &MainWindow::onSocketDisconnected);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &MainWindow::onSocketMessageReceived);
}

void MainWindow::sendMessage()
{
    if (!m_serverConnected) {
        return UserNotifier::showMessage("Вы не подключены к серверу", QMessageBox::Icon::Critical);
    }
    QString messageText = ui->messageTextEdit->toPlainText();
    if (messageText.isEmpty()) {
        return UserNotifier::showMessage("Сообщение не может быть пустым", QMessageBox::Icon::Critical);
    }

    m_webSocket->sendTextMessage(messageText);
    addMessageToChat(m_webSocket->localAddress().toString(), m_webSocket->localPort(), messageText);
}

void MainWindow::addMessageToChat(const QString t_senderAddress, const int t_senderPort, const QString t_textMessage)
{
    QString chatMessages = ui->chatTextBrowser->toPlainText();
    ui->chatTextBrowser->setText(chatMessages +
                                     QString("\nНовое сообщение от %1:%2\n%3").arg(
                                    t_senderAddress, QString::number(t_senderPort), t_textMessage));
}
