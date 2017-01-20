#include "screenlocker.h"
#include "adminloginwidget.h"
#include "settingswidget.h"
#include "loginwidget.h"
#include "connection.h"
#include "global.h"

#include <QApplication>
#include <QBoxLayout>
#include <QAction>
#include <QCloseEvent>
#include <QLabel>
#include <QSettings>
#include <QTimer>
#include <QDebug>

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
    footerLabel->setText(QString("%1 v%2<br>%3").arg(
        qApp->applicationDisplayName(),
        qApp->applicationVersion(),
        "©2017 Shift IT Solution"
    ));

    QBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(clientIdLabel);
    mainLayout->addWidget(companyNameLabel);
    mainLayout->addWidget(companyAddressLabel);
    mainLayout->addStretch(2);
    mainLayout->addLayout(contentLayout, 1);
    mainLayout->addStretch(2);
    mainLayout->addWidget(footerLabel);

    adminLoginAction = new QAction(this);
    adminLoginAction->setShortcut(QKeySequence("Ctrl+Shift+Alt+Z"));
    connect(adminLoginAction, SIGNAL(triggered(bool)), SLOT(showAdminLoginWidget()));
    addAction(adminLoginAction);

    connect(Connection::instance(), SIGNAL(connected()), SLOT(standBy()));
    connect(Connection::instance(), SIGNAL(disconnected()), SLOT(standBy()));
    connect(Connection::instance(), SIGNAL(receiveInit()), SLOT(updateClientInfo()));

    updateClientInfo();
}

void ScreenLocker::updateClientInfo()
{
    QSettings settings(SNBC_SETTINGS_PATH, QSettings::IniFormat);
    clientIdLabel->setText(settings.value(SNBC_SK_CLIENT_ID).toString());
    companyNameLabel->setText(settings.value(SNBC_SK_COMPANY_NAME).toString());
    companyAddressLabel->setText(settings.value(SNBC_SK_COMPANY_ADDRESS).toString());
}

void ScreenLocker::closeEvent(QCloseEvent* event)
{
    event->ignore();
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

void ScreenLocker::showLoginWidget()
{
    if (contentWidget() && contentWidget()->inherits(LoginWidget::staticMetaObject.className()))
        return;

    LoginWidget *widget = new LoginWidget(this);
    connect(widget, SIGNAL(accepted()), SLOT(showTopupWidget()));
    setContentWidget(widget);
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

void ScreenLocker::showTopupWidget()
{

}

void ScreenLocker::standBy()
{
    // abaikan jika admin mau login ato sedang di tampilan settings
    if (sender() == Connection::instance() && contentWidget() &&
        (contentWidget()->inherits(AdminLoginWidget::staticMetaObject.className())
         || contentWidget()->inherits(SettingsWidget::staticMetaObject.className())))
        return;

    if (Connection::instance()->isConnected())
        showLoginWidget();
    else
        showDisconnectedWarningLabel();

    adminLoginAction->setEnabled(true);
}

