#include "billingdialog.h"
#include "ui_billingdialog.h"
#include "application.h"
#include "global.h"
#include "topupdialog.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QTimer>
#include <QDesktopWidget>
#include <QDebug>

BillingDialog::BillingDialog(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint)
    , ui(new Ui::BillingDialog)
    , timer(new QTimer(this))
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowIcon(QIcon(":/shift.ico"));

    ui->setupUi(this);
    timer->setSingleShot(true);
    timer->setInterval(5000);

    ui->companyLabel->setText(qApp->settings(SNBC_SK_COMPANY_NAME).toString());
    ui->clientIdLabel->setText(QString::number(qApp->settings(SNBC_SK_CLIENT_ID).toInt()));
    ui->usernameLabel->setText(qApp->property("username").toString());

    connect(ui->okButton, SIGNAL(clicked(bool)), SLOT(showTopupDialog()));
    connect(ui->exitButton, SIGNAL(clicked(bool)), SLOT(confirmExit()));
    connect(qApp, SIGNAL(durationUpdated()), SLOT(updateDuration()));
    connect(qApp, SIGNAL(sessionStopped()), SLOT(onSessionStopped()));
    connect(qApp, SIGNAL(sessionTimeout()), SLOT(onSessionStopped()));
    connect(qApp, SIGNAL(disconnected()), SLOT(reject()));
    connect(timer, SIGNAL(timeout()), SLOT(onTimeout()));

    updateDuration();

    adjustSize();
    setFixedSize(size());
    move(qApp->desktop()->screenGeometry().right() - width() - 20, 20);
}

BillingDialog::~BillingDialog()
{
    delete ui;
}

void BillingDialog::updateDuration()
{
    QString durationString;
    int duration = qApp->property("duration").toInt();
    int h = duration / 60;
    int m = duration - (h* 60);
    durationString += QString("%1").arg(h, 2, 10, QChar('0'));
    durationString += ":";
    durationString += QString("%1").arg(m, 2, 10, QChar('0'));
    ui->durationLabel->setText(durationString);
}

void BillingDialog::confirmExit()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Konfirmasi");
    msgBox.setText("Anda yakin sudah selesai bermain?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.addButton("&Tidak", QMessageBox::RejectRole);
    msgBox.addButton("&Ya", QMessageBox::AcceptRole);

    connect(qApp, SIGNAL(sessionTimeout()), &msgBox, SLOT(reject()));
    connect(qApp, SIGNAL(disconnected()), &msgBox, SLOT(reject()));

    if (!msgBox.exec())
        return;

    timer->start();
    setEnabled(false);
    qApp->sendSessionStop();
}

void BillingDialog::showTopupDialog()
{
    TopupDialog dialog(this);
    dialog.exec();
}

void BillingDialog::onTimeout()
{
    setEnabled(true);
    QMessageBox::critical(0, "Kesalahan", "Gagal menutup sesi karena batas waktu permintaan ke server telah habis.");
}

void BillingDialog::onSessionStopped()
{
    timer->stop();
    accept();
}

void BillingDialog::closeEvent(QCloseEvent* event)
{
    event->ignore();
}