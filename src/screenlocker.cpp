#include "screenlocker.h"
#include "adminloginwidget.h"
#include "settingswidget.h"
#include "memberloginwidget.h"

#include <QBoxLayout>
#include <QAction>

ScreenLocker::ScreenLocker(QWidget *parent)
    : QWidget(parent)
{
    contentLayout = new QHBoxLayout;
    contentLayout->addStretch(1);
    contentLayout->addStretch(1);

    QBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addStretch(1);
    mainLayout->addLayout(contentLayout);
    mainLayout->addStretch(1);

    adminLoginAction = new QAction(this);
    adminLoginAction->setShortcut(QKeySequence("Ctrl+Shift+Alt+Z"));
    connect(adminLoginAction, SIGNAL(triggered(bool)), SLOT(showAdminLoginWidget()));
    addAction(adminLoginAction);

    showMemberLoginWidget();
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
    adminLoginAction->setEnabled(false);

    AdminLoginWidget* widget = new AdminLoginWidget(this);
    connect(widget, SIGNAL(accepted()), SLOT(showSettingsWidget()));
    connect(widget, SIGNAL(rejected()), SLOT(showMemberLoginWidget()));
    setContentWidget(widget);
}

void ScreenLocker::showSettingsWidget()
{
    SettingsWidget* widget = new SettingsWidget(this);
    connect(widget, SIGNAL(finished()), SLOT(showMemberLoginWidget()));
    setContentWidget(widget);
}

void ScreenLocker::showMemberLoginWidget()
{
    if (contentWidget() && contentWidget()->inherits(MemberLoginWidget::staticMetaObject.className())) {
        return;
    }

    MemberLoginWidget *widget = new MemberLoginWidget(this);
    setContentWidget(widget);

    adminLoginAction->setEnabled(true);
}
