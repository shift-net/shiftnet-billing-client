#include "global.h"
#include "application.h"
#include "screenlocker.h"
#include "adminloginwidget.h"
#include "settingswidget.h"
#include "memberloginwidget.h"
#include "guestloginwidget.h"

#include <QBoxLayout>
#include <QAction>
#include <QCloseEvent>
#include <QLabel>
#include <QSettings>
#include <QTimer>
#include <QPushButton>

void ScreenLockerContentWidget::showError(QWidget *widget, const QString &message)
{
    QToolTip::showText(widget->mapToGlobal(QPoint(widget->width(), -(widget->height() / 2))),
                       "<font style='color:red;'>" + message + "</font>",
                       widget);
}

ScreenLocker::ScreenLocker(QWidget *parent)
    : QWidget(parent, Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    contentLayout = new QHBoxLayout;
    contentLayout->addStretch(1);
    contentLayout->addStretch(1);

    clientIdLabel = new QLabel(this);
    clientIdLabel->setObjectName("clientIdLabel");
    clientIdLabel->setText("#");
    clientIdLabel->setAlignment(Qt::AlignCenter);

    companyNameLabel = new QLabel(this);
    companyNameLabel->setObjectName("companyNameLabel");
    companyNameLabel->setText("ShiftNet");
    companyNameLabel->setAlignment(Qt::AlignCenter);

    companyAddressLabel = new QLabel(this);
    companyAddressLabel->setObjectName("companyAddressLabel");
    companyAddressLabel->setText("Jl. Jend. Ahmad Yani No. 5 Talaga");
    companyAddressLabel->setAlignment(Qt::AlignCenter);

    QLabel* footerLabel = new QLabel(this);
    footerLabel->setObjectName("footerLabel");
    footerLabel->setAlignment(Qt::AlignCenter);
    footerLabel->setText(QString("%1 v%2 - %3").arg(
        qApp->applicationDisplayName(),
        qApp->applicationVersion(),
        "©2017 Shift IT Solution"
    ));

    QPushButton* shutdownButton = new QPushButton("&Shutdown", this);
    shutdownButton->setObjectName("shutdownButton");
    QPushButton* restartButton = new QPushButton("&Restart", this);
    restartButton->setObjectName("restartButton");

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->setMargin(0);
    buttonsLayout->setContentsMargins(0, 0, 0, 15);
    buttonsLayout->setSpacing(10);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(shutdownButton);
    buttonsLayout->addWidget(restartButton);
    buttonsLayout->addStretch(1);

    QBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(clientIdLabel);
    mainLayout->addWidget(companyNameLabel);
    mainLayout->addWidget(companyAddressLabel);
    mainLayout->addStretch(2);
    mainLayout->addLayout(contentLayout, 1);
    mainLayout->addStretch(2);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(footerLabel);

    adminLoginAction = new QAction(this);
    adminLoginAction->setShortcut(QKeySequence("Ctrl+Shift+Alt+Z"));
    addAction(adminLoginAction);
    connect(adminLoginAction, SIGNAL(triggered(bool)), SLOT(showAdminLoginWidget()));

    connect(shutdownButton, SIGNAL(clicked(bool)), qApp, SLOT(shutdownSystem()));
    connect(restartButton, SIGNAL(clicked(bool)), qApp, SLOT(restartSystem()));
    connect(qApp, SIGNAL(connected()), SLOT(standBy()));
    connect(qApp, SIGNAL(disconnected()), SLOT(standBy()));
    connect(qApp, SIGNAL(initialized()), SLOT(updateClientInfo()));

    updateClientInfo();
}

void ScreenLocker::updateClientInfo()
{
    clientIdLabel->setText(qApp->settings(SNBC_SK_CLIENT_ID).toString());
    companyNameLabel->setText(qApp->settings(SNBC_SK_COMPANY_NAME).toString());
    companyAddressLabel->setText(qApp->settings(SNBC_SK_COMPANY_ADDRESS).toString());
}

void ScreenLocker::setContentWidget(QWidget* widget)
{
    QWidget* current = contentWidget();
    if (current) {
        contentLayout->removeWidget(current);
        current->deleteLater();
    }

    if (widget != 0) {
        contentLayout->insertWidget(1, widget);
    }
}

QWidget* ScreenLocker::contentWidget() const
{
    return contentLayout->count() == 2 ? 0 : contentLayout->itemAt(1)->widget();
}

void ScreenLocker::showDisconnectedWarningLabel()
{
    if (contentWidget() && contentWidget()->inherits(QLabel::staticMetaObject.className()))
        return;

    QLabel* label = new QLabel(this);
    label->setObjectName("disconnectedWarningLabel");
    label->setText("Koneksi Terputus");
    label->setAlignment(Qt::AlignCenter);
    label->setFixedSize(300, 80);
    setContentWidget(label);
}

void ScreenLocker::showAdminLoginWidget()
{
    if (contentWidget() && contentWidget()->inherits(AdminLoginWidget::staticMetaObject.className()))
        return;

    adminLoginAction->setEnabled(false);

    AdminLoginWidget* widget = new AdminLoginWidget(this);
    connect(widget, SIGNAL(accepted()), SLOT(showSettingsWidget()));
    connect(widget, SIGNAL(rejected()), SLOT(standBy()));
    setContentWidget(widget);
}

void ScreenLocker::showSettingsWidget()
{
    if (contentWidget() && contentWidget()->inherits(SettingsWidget::staticMetaObject.className()))
        return;

    SettingsWidget* widget = new SettingsWidget(this);
    connect(widget, SIGNAL(finished()), SLOT(standBy()));
    setContentWidget(widget);
}

void ScreenLocker::showMemberLoginWidget()
{
    if (contentWidget() && contentWidget()->inherits(MemberLoginWidget::staticMetaObject.className()))
        return;

    MemberLoginWidget *widget = new MemberLoginWidget(this);
    connect(widget, SIGNAL(accepted()), SLOT(showTopupWidget()));
    connect(widget, SIGNAL(rejected()), SLOT(standBy()));
    setContentWidget(widget);
}

void ScreenLocker::showGuestLoginWidget()
{
    if (contentWidget() && contentWidget()->inherits(GuestLoginWidget::staticMetaObject.className()))
        return;

    GuestLoginWidget *widget = new GuestLoginWidget(this);
    connect(widget, SIGNAL(memberLoginRequested()), SLOT(showMemberLoginWidget()));
    setContentWidget(widget);
}

void ScreenLocker::standBy()
{
    // abaikan jika sedang menampilkan form admin / settings
    if (sender() == Application::instance() && contentWidget() &&
        (contentWidget()->inherits(AdminLoginWidget::staticMetaObject.className())
         || contentWidget()->inherits(SettingsWidget::staticMetaObject.className())))
        return;

    if (qApp->isConnected())
        showGuestLoginWidget();
    else
        showDisconnectedWarningLabel();

    adminLoginAction->setEnabled(true);
}

void ScreenLocker::closeEvent(QCloseEvent* event)
{
    event->ignore();
}
