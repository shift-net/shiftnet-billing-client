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
        showError(ui->passwordEdit, "Silahkan masukkan kata sandi!");
    }
    else if (userPassword != storedPassword) {
        ui->passwordEdit->setFocus();
        ui->passwordEdit->selectAll();
        showError(ui->passwordEdit, "Kata sandi yang anda masukkan salah!");
    }
    else {
        emit accepted();
    }
}

void AdminLoginWidget::showError(QWidget* widget, const QString& message)
{
    QToolTip::showText(widget->mapToGlobal(QPoint(widget->width(), -(widget->height() / 2))),
                       "<font style='color:red;'>" + message + "</font>",
                       widget);
}
