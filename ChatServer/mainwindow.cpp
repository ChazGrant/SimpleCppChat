#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->startServerButton, &QPushButton::clicked, this, &MainWindow::startServer);
    connect(ui->sendMessagePushButton, &QPushButton::clicked, this, &MainWindow::sendMessageToClient);

    m_chatServer = new ChatServerModelView();

    ui->clientOnlineLabel->setText("Клиент не подключен");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startServer()
{
    QString serverPort = ui->serverPortTextEdit->toPlainText();


    QString errorMessage;
    m_chatServer->startServer(serverPort, errorMessage);
    if (!errorMessage.isEmpty()) {
        return UserNotifier::showMessage(errorMessage, QMessageBox::Icon::Critical);
    }

    setServerSignals();
    UserNotifier::showMessage("Сервер запущен", QMessageBox::Icon::Information);
}

void MainWindow::sendMessageToClient()
{
    QString textMessage = ui->chatMessageTextEdit->toPlainText();
    QString errorMessage;
    m_chatServer->sendMessageToClient(textMessage, errorMessage);
    if (!errorMessage.isEmpty()) {
        return UserNotifier::showMessage(errorMessage, QMessageBox::Icon::Critical);
    }

    QString senderName = m_chatServer->getSenderName();
    addMessageToChat(senderName, textMessage);
    ui->chatMessageTextEdit->clear();
}

void MainWindow::onSocketConnected()
{
    updateEvents("Клиент подключился");
    ui->clientOnlineLabel->setText("Клиент подключен");
}

void MainWindow::onSocketDisconnected()
{
    updateEvents("Клиент отключился");
    ui->clientOnlineLabel->setText("Клиент отключён");
}

void MainWindow::addMessageToChat(const QString t_senderName, const QString t_textMessage)
{
    QString chatMessages = ui->chatTextBrowser->toPlainText();
    ui->chatTextBrowser->setText(chatMessages +
                                QString("\nНовое сообщение от %1\n%2").arg(
                                    t_senderName, t_textMessage));
}

void MainWindow::updateEvents(const QString t_newEvent)
{
    QString shownEvents = ui->eventsTextBrowser->toPlainText();
    ui->eventsTextBrowser->setText(shownEvents + "\n" + t_newEvent);
}

void MainWindow::setServerSignals()
{
    disconnect(m_chatServer, &ChatServerModelView::clientConnected, this, &MainWindow::onSocketConnected);
    disconnect(m_chatServer, &ChatServerModelView::clientDisconnected, this, &MainWindow::onSocketDisconnected);
    disconnect(m_chatServer, &ChatServerModelView::messsageReceived, this, &MainWindow::addMessageToChat);

    connect(m_chatServer, &ChatServerModelView::clientConnected, this, &MainWindow::onSocketConnected);
    connect(m_chatServer, &ChatServerModelView::clientDisconnected, this, &MainWindow::onSocketDisconnected);
    connect(m_chatServer, &ChatServerModelView::messsageReceived, this, &MainWindow::addMessageToChat);
}
