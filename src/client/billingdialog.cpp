#include "billingdialog.h"
#include "ui_billingdialog.h"
#include "application.h"
#include "global.h"
#include "topupdialog.h"

#include <qevent.h>
#include <QMessageBox>
#include <QTimer>
#include <QDesktopWidget>

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
    msgBox.setIconPixmap(QIcon(":/fa-question-black.png").pixmap(QSize(32, 32)));
    QPushButton* btn = msgBox.addButton("&Tidak", QMessageBox::RejectRole);
    btn->setIcon(QIcon(":/fa-close-black.png"));
    btn = msgBox.addButton("&Ya", QMessageBox::AcceptRole);
    btn->setIcon(QIcon(":/fa-check-black.png"));

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

    QMessageBox msgBox;
    msgBox.setWindowTitle("Kesalahan");
    msgBox.setIconPixmap(QIcon(":/fa-info-black.png").pixmap(QSize(32, 32)));
    msgBox.setText(QString("Gagal menutup sesi karena batas waktu permintaan ke server telah habis."));
    msgBox.addButton(QMessageBox::Ok);
    QTimer::singleShot(5000, &msgBox, SLOT(accept()));
    msgBox.exec();
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

void BillingDialog::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
}
