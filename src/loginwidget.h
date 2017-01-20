#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include "screenlocker.h"

namespace Ui {
    class LoginWidget;
}

class LoginWidget : public ScreenLockerContentWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

private slots:
    void accept();
    void reset();
    void processLogin(const QJsonObject& data);
    void loginTimeout();

signals:
    void accepted();

private:
    void showError(QWidget* widget, const QString& message);
    Ui::LoginWidget *ui;
    QTimer* loginTimer;
};

#endif // LOGINWIDGET_H
