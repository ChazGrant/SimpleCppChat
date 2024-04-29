#include "usernotifier.h"

UserNotifier::UserNotifier()
{

}

void UserNotifier::showMessage(const QString t_messageText, const QMessageBox::Icon t_messageIcon)
{
    QMessageBox msgBox;
    msgBox.setIcon(t_messageIcon);
    msgBox.setText(t_messageText);
    if (t_messageIcon == QMessageBox::Icon::Critical) {
        msgBox.setWindowTitle("Ошибка");
    } else if (t_messageIcon == QMessageBox::Icon::Information) {
        msgBox.setWindowTitle("Информация");
    }

    msgBox.exec();
}
