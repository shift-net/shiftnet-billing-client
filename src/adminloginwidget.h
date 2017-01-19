#ifndef ADMINLOGINWIDGET_H
#define ADMINLOGINWIDGET_H

#include "screenlocker.h"

namespace Ui {
    class AdminLoginWidget;
}

class AdminLoginWidget : public ScreenLockerContentWidget
{
    Q_OBJECT

public:
    explicit AdminLoginWidget(QWidget *parent = 0);
    ~AdminLoginWidget();

private slots:
    void accept();

signals:
    void accepted();
    void rejected();

private:
    void showError(QWidget* widget, const QString& message);
    Ui::AdminLoginWidget *ui;
};

#endif // ADMINLOGINWIDGET_H
