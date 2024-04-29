#ifndef USERNOTIFIER_H
#define USERNOTIFIER_H

#include "QMessageBox"


class UserNotifier
{
public:
    UserNotifier();

    static void showMessage(const QString t_messageText, const QMessageBox::Icon t_messageIcon);
};

#endif // USERNOTIFIER_H
