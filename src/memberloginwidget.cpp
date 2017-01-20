#include "memberloginwidget.h"
#include "ui_memberloginwidget.h"
#include "global.h"

#include <QToolTip>
#include <QTimer>
#include <QAction>
#include <QSettings>
#include <QApplication>

MemberLoginWidget::MemberLoginWidget(QWidget *parent)
    : ScreenLockerContentWidget(parent)
    , ui(new Ui::MemberLoginWidget)
{
    ui->setupUi(this);

    QAction* rejectAction = new QAction(this);
    rejectAction->setShortcut(QKeySequence("Esc"));
    addAction(rejectAction);

    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->resetButton, SIGNAL(clicked()), SLOT(reset()));
    connect(ui->codeEdit, SIGNAL(returnPressed()), SLOT(accept()));
    connect(ui->passwordEdit, SIGNAL(returnPressed()), SLOT(accept()));
    connect(rejectAction, SIGNAL(triggered(bool)), SLOT(reset()));

    QTimer::singleShot(0, ui->codeEdit, SLOT(setFocus()));
}

MemberLoginWidget::~MemberLoginWidget()
{
    delete ui;
}

void MemberLoginWidget::accept()
{
    if (ui->codeEdit->text().isEmpty()) {
        ui->codeEdit->setFocus();
        showError(ui->codeEdit, "Silahkan masukkan ID / Kode Voucher!");
        return;
    }

    if (ui->passwordEdit->text().isEmpty()) {
        ui->passwordEdit->setFocus();
        showError(ui->passwordEdit, "Silahkan masukkan kata sandi!");
        return;
    }

    // TODO: Implementasi login ke server

    emit accepted();
}

void MemberLoginWidget::reset()
{
    ui->codeEdit->clear();
    ui->passwordEdit->clear();
    ui->codeEdit->setFocus();
}

void MemberLoginWidget::showError(QWidget* widget, const QString& message)
{
    QToolTip::showText(widget->mapToGlobal(QPoint(widget->width(), -(widget->height() / 2))),
                       "<font style='color:red;'>" + message + "</font>",
                       widget);
}
