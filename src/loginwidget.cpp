#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "global.h"
#include "connection.h"

#include <QJSonObject>
#include <QToolTip>
#include <QTimer>
#include <QAction>
#include <QSettings>
#include <QApplication>
#include <QTimer>

LoginWidget::LoginWidget(QWidget *parent)
    : ScreenLockerContentWidget(parent)
    , ui(new Ui::LoginWidget)
    , loginTimer(new QTimer(this))
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
    connect(Connection::instance(), SIGNAL(receiveLogin(QJsonObject)), SLOT(processLogin(QJsonObject)));
    connect(loginTimer, SIGNAL(timeout()), SLOT(onLoginTimeout()));

    QTimer::singleShot(0, ui->codeEdit, SLOT(setFocus()));
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::accept()
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

    Connection::instance()->requestLogin(ui->codeEdit->text(), ui->passwordEdit->text());
    setEnabled(false);

    loginTimer->start(5000);
}

void LoginWidget::reset()
{
    ui->codeEdit->clear();
    ui->passwordEdit->clear();
    ui->codeEdit->setFocus();
}

void LoginWidget::showError(QWidget* widget, const QString& message)
{
    QToolTip::showText(widget->mapToGlobal(QPoint(widget->width(), -(widget->height() / 2))),
                       "<font style='color:red;'>" + message + "</font>",
                       widget);
}

void LoginWidget::processLogin(const QJsonObject& data)
{
    loginTimer->stop();
    setEnabled(true);

    if (data.value("success").toBool()) {
        emit accepted();
        return;
    }

    QJsonObject errors = data.value("errors").toObject();
    if (errors.contains("id")) {
        ui->codeEdit->setFocus();
        showError(ui->codeEdit, data.value("id").toString());
        return;
    }

    if (errors.contains("password")) {
        ui->passwordEdit->setFocus();
        showError(ui->passwordEdit, data.value("password").toString());
        return;
    }
}


void LoginWidget::onLoginTimeout()
{
    loginTimer->stop();
    setEnabled(true);
    showError(ui->okButton, "Login timeout!!!");
}
