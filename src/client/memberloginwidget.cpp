#include "ui_memberloginwidget.h"
#include "memberloginwidget.h"
#include "global.h"
#include "application.h"

#include <QJSonObject>
#include <QToolTip>
#include <QTimer>
#include <QAction>
#include <QSettings>
#include <QApplication>
#include <QTimer>

MemberLoginWidget::MemberLoginWidget(QWidget *parent)
    : ScreenLockerContentWidget(parent)
    , ui(new Ui::MemberLoginWidget)
    , loginTimer(new QTimer(this))
{
    ui->setupUi(this);

    loginTimer->setSingleShot(true);

    ui->usernameEdit->setClearButtonEnabled(true);
    ui->passwordEdit->setClearButtonEnabled(true);
    ui->voucherCodeEdit->setClearButtonEnabled(true);

    QAction* rejectAction = new QAction(this);
    rejectAction->setShortcut(QKeySequence("Esc"));
    addAction(rejectAction);

    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->usernameEdit, SIGNAL(returnPressed()), SLOT(accept()));
    connect(ui->passwordEdit, SIGNAL(returnPressed()), SLOT(accept()));
    connect(ui->voucherCodeEdit, SIGNAL(returnPressed()), SLOT(accept()));
    connect(ui->commandLinkButton, SIGNAL(clicked(bool)), SIGNAL(rejected()));
    connect(rejectAction, SIGNAL(triggered(bool)), SIGNAL(rejected()));
    connect(qApp, SIGNAL(memberLoginFailed(QString,QString)), SLOT(onLoginFailed(QString,QString)));
    connect(qApp, SIGNAL(sessionStarted()), loginTimer, SLOT(stop()));
    connect(loginTimer, SIGNAL(timeout()), SLOT(onLoginTimeout()));

    QTimer::singleShot(0, ui->usernameEdit, SLOT(setFocus()));
}

MemberLoginWidget::~MemberLoginWidget()
{
    delete ui;
}

void MemberLoginWidget::accept()
{
    const QString username = ui->usernameEdit->text();
    if (username.isEmpty()) {
        ui->usernameEdit->setFocus();
        showError(ui->usernameEdit, "Silahkan masukkan nama pengguna!");
        return;
    }

    const QString password = ui->passwordEdit->text();
    if (password.isEmpty()) {
        ui->passwordEdit->setFocus();
        showError(ui->passwordEdit, "Silahkan masukkan kata sandi!");
        return;
    }

    loginTimer->start(5000);

    setEnabled(false);

    qApp->sendMemberLogin(username, password, ui->voucherCodeEdit->text());
}

void MemberLoginWidget::onLoginFailed(const QString& field, const QString& message)
{
    loginTimer->stop();

    setEnabled(true);

    if (field == "username") {
        ui->usernameEdit->setFocus();
        showError(ui->usernameEdit, message);
        return;
    }

    if (field == "password") {
        ui->passwordEdit->setFocus();
        showError(ui->passwordEdit, message);
        return;
    }

    if (field == "voucherCode") {
        ui->voucherCodeEdit->setFocus();
        showError(ui->voucherCodeEdit, message);
        return;
    }
}


void MemberLoginWidget::onLoginTimeout()
{
    loginTimer->stop();

    setEnabled(true);

    showError(ui->okButton, "Login timeout!!!");
}
