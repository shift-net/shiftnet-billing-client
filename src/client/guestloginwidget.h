#ifndef GUESTLOGINWIDGET_H
#define GUESTLOGINWIDGET_H

#include "screenlocker.h"
#include <QVariant>

namespace Ui {
    class GuestLoginWidget;
}

class GuestLoginWidget : public ScreenLockerContentWidget
{
    Q_OBJECT

public:
    explicit GuestLoginWidget(QWidget *parent = 0);
    ~GuestLoginWidget();

private slots:
    void onSubmit();
    void onTimeout();
    void onError(const QString& msg);

signals:
    void accepted();
    void memberLoginRequested();

private:
    Ui::GuestLoginWidget *ui;
    QTimer* loginTimer;
};

#endif // GUESTLOGINWIDGET_H
