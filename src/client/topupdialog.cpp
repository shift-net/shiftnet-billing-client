#include "topupdialog.h"
#include "ui_topupdialog.h"
#include "application.h"
#include <QMessageBox>
#include <QToolTip>
#include <QDesktopWidget>

TopupDialog::TopupDialog(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint)
    , ui(new Ui::TopupDialog)
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground, true);

    ui->voucherCodeEdit->setClearButtonEnabled(true);

    timer.setSingleShot(true);

    connect(ui->okButton, SIGNAL(clicked(bool)), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
    connect(qApp, SIGNAL(sessionTimeout()), SLOT(reject()));
    connect(qApp, SIGNAL(disconnected()), SLOT(reject()));
    connect(qApp, SIGNAL(topupFailed(QString)), SLOT(onTopupFailed(QString)));
    connect(qApp, SIGNAL(topupSuccess(int)), SLOT(onTopupSuccess(int)));
    connect(&timer, SIGNAL(timeout()), SLOT(onTimeout()));

    adjustSize();
    setFixedSize(size());
    QRect desktopRect = qApp->desktop()->screenGeometry();
    move((desktopRect.width() - width()) / 2, (desktopRect.height() - height()) / 2);

    QTimer::singleShot(0, ui->voucherCodeEdit, SLOT(setFocus()));
}

TopupDialog::~TopupDialog()
{
    delete ui;
}

void TopupDialog::accept()
{
    const QString code = ui->voucherCodeEdit->text();
    if (code.isEmpty()) {
        showError("Silahkan masukkan kode voucher.");
        return;
    }

    qApp->sendTopup(code);
    timer.start(5000);
    setEnabled(false);
}

void TopupDialog::onTimeout()
{
    onTopupFailed("Batas waktu permintaan topup telah habis.");
}

void TopupDialog::onTopupFailed(const QString &msg)
{
    timer.stop();
    setEnabled(true);
    showError(msg);
}

void TopupDialog::onTopupSuccess(int duration)
{
    hide();

    QString durationString;
    int h = duration / 60;
    int m = duration - (h* 60);
    durationString += QString("%1").arg(h, 2, 10, QChar('0'));
    durationString += ":";
    durationString += QString("%1").arg(m, 2, 10, QChar('0'));

    timer.stop();
    setEnabled(true);

    QMessageBox msgBox;
    msgBox.setWindowTitle("Informasi");
    msgBox.setIconPixmap(QIcon(":/fa-info-black.png").pixmap(QSize(32, 32)));
    msgBox.setText(QString("Topup voucher berhasil. Waktu voucher %1 telah ditambahkan.").arg(durationString));
    msgBox.addButton(QMessageBox::Ok);
    QTimer::singleShot(5000, &msgBox, SLOT(accept()));
    msgBox.exec();

    QDialog::accept();
}

void TopupDialog::showError(const QString& msg)
{
    ui->voucherCodeEdit->setFocus();
    ui->voucherCodeEdit->clear();
    QToolTip::showText(ui->voucherCodeEdit->mapToGlobal(QPoint(ui->voucherCodeEdit->width(), -(ui->voucherCodeEdit->height() / 2))),
                       "<font style='color:red;'>" + msg + "</font>",
                       ui->voucherCodeEdit);
}
