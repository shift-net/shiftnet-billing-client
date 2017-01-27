#include "ui_settingswidget.h"
#include "settingswidget.h"
#include "global.h"
#include "application.h"

#include <QTimer>
#include <QAction>
#include <QRegExpValidator>
#include <QIntValidator>

SettingsWidget::SettingsWidget(QWidget *parent)
    : ScreenLockerContentWidget(parent)
    , ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    QAction* rejectAction = new QAction(this);
    rejectAction->setShortcut(QKeySequence("Esc"));
    addAction(rejectAction);

    ui->ipAddressEdit->setValidator(new QRegExpValidator(QRegExp("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
                                                                 "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"), this));
    ui->portEdit->setValidator(new QIntValidator(0, 65535, this));

    ui->ipAddressEdit->setText(qApp->settings(SNBC_SK_SERVER_ADDRESS, SNBC_DEFAULT_SERVER_ADDRESS).toString());
    ui->portEdit->setText(QString::number(qApp->settings(SNBC_SK_SERVER_PORT, SNBC_DEFAULT_SERVER_PORT).toInt()));

    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SIGNAL(finished()));
    connect(ui->exitButton, SIGNAL(clicked()), qApp, SLOT(maintenance()));
    connect(ui->ipAddressEdit, SIGNAL(returnPressed()), SLOT(accept()));
    connect(ui->portEdit, SIGNAL(returnPressed()), SLOT(accept()));
    connect(rejectAction, SIGNAL(triggered(bool)), SIGNAL(finished()));

    QTimer::singleShot(0, ui->ipAddressEdit, SLOT(setFocus()));
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::accept()
{
    if (ui->ipAddressEdit->text().isEmpty()) {
        ui->ipAddressEdit->setFocus();
        showError(ui->portEdit, "Silahkan masukkan alamat IP server!");
        return;
    }

    if (ui->portEdit->text().isEmpty()) {
        ui->portEdit->setFocus();
        showError(ui->portEdit, "Silahkan masukkan port server!");
        return;
    }

    QString enteredIpAddress = ui->ipAddressEdit->text();
    int pos = enteredIpAddress.size() - 1;
    if (QValidator::Acceptable != ui->ipAddressEdit->validator()->validate(enteredIpAddress, pos)) {
        ui->ipAddressEdit->setFocus();
        showError(ui->ipAddressEdit, "Format alamat IP server tidak benar!");
        return;
    }

    int enteredPort = ui->portEdit->text().toInt();

    qApp->setSettings(SNBC_SK_SERVER_ADDRESS, enteredIpAddress);
    qApp->setSettings(SNBC_SK_SERVER_PORT, enteredPort);

    emit finished();
}
