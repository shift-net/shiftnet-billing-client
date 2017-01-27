#ifndef SCREENLOCKER_H
#define SCREENLOCKER_H

#include <QFrame>
#include <QToolTip>

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
    void showGuestLoginWidget();
    void showMemberLoginWidget();
    void showDisconnectedWarningLabel();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void updateClientInfo();

private:
    QBoxLayout* contentLayout;
    QAction* adminLoginAction;
    QLabel* clientIdLabel;
    QLabel* companyNameLabel;
    QLabel* companyAddressLabel;
    QLabel* systemCommandsLabel;
};

class ScreenLockerContentWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ScreenLockerContentWidget(QWidget* = 0) {}

    virtual void showError(QWidget* widget, const QString& message);
};

#endif // SCREENLOCKER_H
