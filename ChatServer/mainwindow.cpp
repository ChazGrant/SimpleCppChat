#include "mainwindow.h"
#include "ui_mainwindow.h"


//! @brief Конструктор класса
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

//! @brief Деструктор класса
MainWindow::~MainWindow()
{
    delete ui;
}

/*! @brief Запуск сервера
 *
 *  @return void
*/
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

/*! @brief Отправка сообщения клиенту
 *
 *  @return void
*/
void MainWindow::sendMessageToClient()
{
    QString textMessage = ui->chatMessageTextEdit->toPlainText();
    QString errorMessage;
    m_chatServer->sendMessageToClient(textMessage, errorMessage);
    if (!errorMessage.isEmpty()) {
        return UserNotifier::showMessage(errorMessage, QMessageBox::Icon::Critical);
    }

    ui->chatMessageTextEdit->clear();
}

/*! @brief Событие при подключении клиента к серверу
 *
 *  @return void
*/
void MainWindow::onSocketConnected()
{
    updateEvents("Клиент подключился");
    ui->clientOnlineLabel->setText("Клиент подключен");
}

/*! @brief Событие при отключении клиента от сервера
 *
 *  @return void
*/
void MainWindow::onSocketDisconnected()
{
    if (m_chatServer->getLastSentMessage().size()) {
        UserNotifier::showMessage("Последнее сообщение не было доставлено из-за обрыва соединения", QMessageBox::Icon::Critical);
    }
    updateEvents("Клиент отключился");
    ui->clientOnlineLabel->setText("Клиент отключён");
}

/*! @brief Добавления сообщения в чат
 *
 *  @param t_senderName Имя отправителя
 *  @param t_textMessage Текст сообщения
 *
 *  @return void
*/
void MainWindow::updateChatMessages(const QString t_senderName, const QString t_textMessage)
{
    QString chatMessages = ui->chatTextBrowser->toPlainText();
    ui->chatTextBrowser->setText(chatMessages +
                                QString("\nНовое сообщение от %1\n%2").arg(
                                    t_senderName, t_textMessage));
}

/*! @brief Обновление событий сервера
 *
 *  @param t_newEvent Текст нового события
 *
 *  @return void
*/
void MainWindow::updateEvents(const QString t_newEvent)
{
    QString shownEvents = ui->eventsTextBrowser->toPlainText();
    ui->eventsTextBrowser->setText(shownEvents + "\n" + t_newEvent);
}

/*! @brief Установка сигналов сервера
 *
 *  @return void
*/
void MainWindow::setServerSignals()
{
    disconnect(m_chatServer, &ChatServerModelView::clientConnected, this, &MainWindow::onSocketConnected);
    disconnect(m_chatServer, &ChatServerModelView::clientDisconnected, this, &MainWindow::onSocketDisconnected);
    disconnect(m_chatServer, &ChatServerModelView::messsageReceived, this, &MainWindow::updateChatMessages);
    disconnect(m_chatServer, &ChatServerModelView::clientReceivedMessage, this, &MainWindow::updateChatMessages);

    connect(m_chatServer, &ChatServerModelView::clientConnected, this, &MainWindow::onSocketConnected);
    connect(m_chatServer, &ChatServerModelView::clientDisconnected, this, &MainWindow::onSocketDisconnected);
    connect(m_chatServer, &ChatServerModelView::messsageReceived, this, &MainWindow::updateChatMessages);
    connect(m_chatServer, &ChatServerModelView::clientReceivedMessage, this, &MainWindow::updateChatMessages);
}
