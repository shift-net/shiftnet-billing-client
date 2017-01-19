#include "ui_adminloginwidget.h"
#include "adminloginwidget.h"
#include "global.h"

#include <QToolTip>
#include <QTimer>
#include <QAction>
#include <QSettings>

AdminLoginWidget::AdminLoginWidget(QWidget *parent)
    : ScreenLockerContentWidget(parent)
    , ui(new Ui::AdminLoginWidget)
{
    ui->setupUi(this);

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
    QSettings settings(SNBC_SETTINGS_PATH, QSettings::IniFormat);
    const QString storedPassword = settings.value(SNBC_SK_ADMIN_PASSWORD, SNBC_DEFAULT_ADMIN_PASSWORD).toString();
    const QString userPassword = ui->passwordEdit->text();

    if (userPassword.isEmpty()) {
        ui->passwordEdit->setFocus();
        QToolTip::showText(ui->passwordEdit->mapToGlobal(QPoint(ui->passwordEdit->width(), -ui->passwordEdit->height() / 2)),
                           "<font style='color:red;'>Kata Sandi harus diisi.</font>",
                           ui->passwordEdit);
    }
    else if (userPassword != storedPassword) {
        ui->passwordEdit->setFocus();
        QToolTip::showText(ui->passwordEdit->mapToGlobal(QPoint(ui->passwordEdit->width(), -ui->passwordEdit->height() / 2)),
                           "<font style='color:red;'>Kata Sandi salah.</font>",
                           ui->passwordEdit);
    }
    else {
        emit accepted();
    }
}
