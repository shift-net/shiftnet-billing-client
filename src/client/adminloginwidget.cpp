#include "ui_adminloginwidget.h"
#include "global.h"
#include "adminloginwidget.h"
#include "application.h"

#include <QTimer>
#include <QAction>
#include <QCryptographicHash>

AdminLoginWidget::AdminLoginWidget(QWidget *parent)
    : ScreenLockerContentWidget(parent)
    , ui(new Ui::AdminLoginWidget)
{
    ui->setupUi(this);

    ui->passwordEdit->setClearButtonEnabled(true);

    QAction* rejectAction = new QAction(this);
    rejectAction->setShortcut(QKeySequence("Esc"));
    addAction(rejectAction);

    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SIGNAL(rejected()));
    connect(ui->passwordEdit, SIGNAL(returnPressed()), SLOT(accept()));
    connect(rejectAction, SIGNAL(triggered(bool)), SIGNAL(rejected()));

    QTimer::singleShot(0, ui->passwordEdit, SLOT(setFocus()));
}

AdminLoginWidget::~AdminLoginWidget()
{
    delete ui;
}

void AdminLoginWidget::accept()
{
    const QByteArray storedPassword = qApp->settings(SNBC_SK_CLIENT_PASSWORD, SNBC_DEFAULT_ADMIN_PASSWORD).toByteArray();
    const QByteArray userPassword = QCryptographicHash::hash(ui->passwordEdit->text().toUtf8(), QCryptographicHash::Sha1).toHex();

    if (userPassword != storedPassword) {
        ui->passwordEdit->setFocus();
        ui->passwordEdit->selectAll();
        showError(ui->passwordEdit, "Kata sandi yang anda masukkan salah!");
        return;
    }

    emit accepted();
}
