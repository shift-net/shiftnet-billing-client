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
    void reset();

signals:
    void accepted();

private:
    void showError(QWidget* widget, const QString& message);
    Ui::MemberLoginWidget *ui;
};

#endif // MEMBERLOGINWIDGET_H
