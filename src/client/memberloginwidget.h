#ifndef MEMBERLOGINWIDGET_H
#define MEMBERLOGINWIDGET_H

#include "screenlocker.h"

namespace Ui {
    class MemberLoginWidget;
}

class MemberLoginWidget : public ScreenLockerContentWidget
{
    Q_OBJECT

public:
    explicit MemberLoginWidget(QWidget *parent = 0);
    ~MemberLoginWidget();

private slots:
    void accept();
    void onLoginFailed(const QString& field, const QString& message);
    void onLoginTimeout();

signals:
    void accepted();
    void rejected();

private:
    Ui::MemberLoginWidget *ui;
    QTimer* loginTimer;
};

#endif // MEMBERLOGINWIDGET_H
