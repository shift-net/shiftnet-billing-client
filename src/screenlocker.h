#ifndef SCREENLOCKER_H
#define SCREENLOCKER_H

#include <QFrame>

class QBoxLayout;
class QLabel;

class ScreenLocker : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenLocker(QWidget *parent = 0);

    void setContentWidget(QWidget* widget);
    QWidget* contentWidget() const;

signals:

public slots:
    void standBy();
    void showAdminLoginWidget();
    void showSettingsWidget();
    void showLoginWidget();
    void showTopupWidget();
    void showDisconnectedWarningLabel();

private slots:
    void updateClientInfo();

protected:
    void closeEvent(QCloseEvent* event);

private:
    QBoxLayout* contentLayout;
    QAction* adminLoginAction;
    QLabel* clientIdLabel;
    QLabel* companyNameLabel;
    QLabel* companyAddressLabel;
};

class ScreenLockerContentWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ScreenLockerContentWidget(QWidget* = 0) {}
};

#endif // SCREENLOCKER_H
