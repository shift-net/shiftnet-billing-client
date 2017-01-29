#include "ui_guestloginwidget.h"
#include "global.h"
#include "guestloginwidget.h"
#include "application.h"

#include <QDateTime>
#include <QTimer>
#include <QAction>
#include <QRegExpValidator>

GuestLoginWidget::GuestLoginWidget(QWidget *parent)
    : ScreenLockerContentWidget(parent)
    , ui(new Ui::GuestLoginWidget)
    , loginTimer(new QTimer(this))
{
    ui->setupUi(this);

    loginTimer->setSingleShot(true);

    QAction* rejectAction = new QAction(this);
    rejectAction->setShortcut(QKeySequence("Esc"));
    addAction(rejectAction);

    QRegExpValidator *usernameValidator = new QRegExpValidator(QRegExp("^[a-zA-Z]+[a-zA-Z0-9\\s]*$"), this);
    ui->usernameEdit->setValidator(usernameValidator);

    QRegExpValidator *voucherValidator = new QRegExpValidator(QRegExp("^[a-zA-Z0-9]{6}$"), this);
    ui->codeEdit->setValidator(voucherValidator);

    connect(ui->commandLinkButton, SIGNAL(clicked(bool)), SIGNAL(memberLoginRequested()));
    connect(ui->okButton, SIGNAL(clicked()), SLOT(onSubmit()));
    connect(ui->usernameEdit, SIGNAL(returnPressed()), SLOT(onSubmit()));
    connect(ui->codeEdit, SIGNAL(returnPressed()), SLOT(onSubmit()));
    connect(rejectAction, SIGNAL(triggered(bool)), ui->codeEdit, SLOT(clear()));
    connect(loginTimer, SIGNAL(timeout()), SLOT(onTimeout()));
    connect(qApp, SIGNAL(sessionStarted()), loginTimer, SLOT(stop()));
    connect(qApp, SIGNAL(guestLoginFailed(QString)), SLOT(onError(QString)));

    QTimer::singleShot(0, ui->usernameEdit, SLOT(setFocus()));
}

GuestLoginWidget::~GuestLoginWidget()
{
    delete ui;
}

void GuestLoginWidget::onSubmit()
{
    const QString username = ui->usernameEdit->text().trimmed();

    if (username.isEmpty() || username.length() < 2) {
        ui->usernameEdit->setFocus();
        showError(ui->usernameEdit, "Silahkan masukkan nama Anda, minimal 2 huruf");
        return;
    }

    const QString code = ui->codeEdit->text();

    if (code.isEmpty()) {
        ui->codeEdit->setFocus();
        showError(ui->codeEdit, "Silahkan masukkan Kode Voucher!");
        return;
    }

    loginTimer->start(5000);

    setEnabled(false);

    qApp->sendGuestLogin(username, code);
}

void GuestLoginWidget::onTimeout()
{
    loginTimer->stop();

    setEnabled(true);

    showError(ui->okButton, "Kesalahan koneksi");
}

void GuestLoginWidget::onError(const QString& msg)
{
    loginTimer->stop();

    setEnabled(true);

    ui->codeEdit->setFocus();
    ui->codeEdit->selectAll();

    showError(ui->codeEdit, msg);
}
