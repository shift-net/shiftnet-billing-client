#include "maintenancedialog.h"
#include "ui_maintenancedialog.h"
#include "application.h"
#include "global.h"
#include "topupdialog.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QTimer>
#include <QDesktopWidget>
#include <QDebug>

MaintenanceDialog::MaintenanceDialog(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint)
    , ui(new Ui::MaintenanceDialog)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowIcon(QIcon(":/shift.ico"));

    ui->setupUi(this);

    ui->companyLabel->setText(qApp->settings(SNBC_SK_COMPANY_NAME).toString());
    ui->clientIdLabel->setText(QString::number(qApp->settings(SNBC_SK_CLIENT_ID).toInt()));
    ui->usernameLabel->setText("Administrator");

    connect(ui->okButton, SIGNAL(clicked(bool)), SLOT(accept()));

    adjustSize();
    setFixedSize(size());
    move(qApp->desktop()->screenGeometry().right() - width() - 20, 20);
}

MaintenanceDialog::~MaintenanceDialog()
{
    delete ui;
}

void MaintenanceDialog::closeEvent(QCloseEvent* event)
{
    event->ignore();
}
