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

void MainWindow::connectToServer()
{
    QString serverAddress = ui->serverAddressTextEdit->toPlainText();
    QString serverPort = ui->serverPortTextEdit->toPlainText();

    if (m_chatClient == nullptr) {
        m_chatClient = new ChatClientModelView();
    }
    QString errorMessage;
    m_chatClient->connectToServer(serverAddress, serverPort, errorMessage);
    if (!errorMessage.isEmpty()) {
        return UserNotifier::showMessage(errorMessage, QMessageBox::Icon::Critical);
    }

    setModelViewSignals();
}

void MainWindow::disconnectFromServer()
{
    QString errorMessage;
    m_chatClient->disconnectFromServer(errorMessage);
    if (!errorMessage.isEmpty()) {
        UserNotifier::showMessage(errorMessage, QMessageBox::Icon::Critical);
    }
}

void MainWindow::onChatSocketConnected()
{
    ui->serverConnectedLabel->setText("Подключение установлено");
}

void MainWindow::onChatSocketDisconnected()
{
    ui->serverConnectedLabel->setText("Подключение не установлено");
}

void MainWindow::sendMessage()
{
    QString errorMessage;
    QString messageText = ui->messageTextEdit->toPlainText();
    m_chatClient->sendMessage(messageText, errorMessage);
    if (!errorMessage.isEmpty()) {
        return UserNotifier::showMessage(errorMessage, QMessageBox::Icon::Critical);
    }

    ui->messageTextEdit->clear();
}

void MainWindow::updateChatMessages(const QString t_senderName, QString t_textMessage)
{
    QString chatMessages = ui->chatTextBrowser->toPlainText();
    ui->chatTextBrowser->setText(chatMessages + QString("\nНовое сообщение от %1\n%2").
                                 arg(t_senderName, t_textMessage));
}

void MainWindow::setModelViewSignals()
{
    disconnect(m_chatClient, &ChatClientModelView::chatSocketConnected, this, &MainWindow::onChatSocketConnected);
    disconnect(m_chatClient, &ChatClientModelView::chatSocketDisconnected, this, &MainWindow::onChatSocketDisconnected);
    disconnect(m_chatClient, &ChatClientModelView::chatSocketMessageReceived, this, &MainWindow::updateChatMessages);
    disconnect(m_chatClient, &ChatClientModelView::serverReceivedMessage, this, &MainWindow::updateChatMessages);

    connect(m_chatClient, &ChatClientModelView::chatSocketConnected, this, &MainWindow::onChatSocketConnected);
    connect(m_chatClient, &ChatClientModelView::chatSocketDisconnected, this, &MainWindow::onChatSocketDisconnected);
    connect(m_chatClient, &ChatClientModelView::chatSocketMessageReceived, this, &MainWindow::updateChatMessages);
    connect(m_chatClient, &ChatClientModelView::serverReceivedMessage, this, &MainWindow::updateChatMessages);
}
