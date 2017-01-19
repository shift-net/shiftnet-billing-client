#include "settingswidget.h"
#include "ui_settingswidget.h"
#include "global.h"

#include <QToolTip>
#include <QTimer>
#include <QAction>
#include <QSettings>

SettingsWidget::SettingsWidget(QWidget *parent)
    : ScreenLockerContentWidget(parent)
    , ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    QAction* rejectAction = new QAction(this);
    rejectAction->setShortcut(QKeySequence("Esc"));
    addAction(rejectAction);

    QSettings settings(SNBC_SETTINGS_PATH, QSettings::IniFormat);
    ui->ipAddressEdit->setText(settings.value(SNBC_SK_SERVER_ADDRESS, SNBC_DEFAULT_SERVER_ADDRESS).toString());
    ui->portEdit->setText(QString::number(settings.value(SNBC_SK_SERVER_PORT, SNBC_DEFAULT_SERVER_PORT).toInt()));

    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SIGNAL(finished()));
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
    emit finished();
}
