#ifndef SCREENLOCKER_H
#define SCREENLOCKER_H

#include <QFrame>

class QBoxLayout;

class ScreenLocker : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenLocker(QWidget *parent = 0);

    void setContentWidget(QWidget* widget);
    QWidget* contentWidget() const;

signals:

public slots:
    void showAdminLoginWidget();
    void showSettingsWidget();
    void exitSettings();

private:
    QBoxLayout* contentLayout;
    QAction* adminLoginAction;
};

class ScreenLockerContentWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ScreenLockerContentWidget(QWidget* = 0) {}
};

#endif // SCREENLOCKER_H
