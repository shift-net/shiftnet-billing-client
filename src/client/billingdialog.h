#ifndef BILLINGDIALOG_H
#define BILLINGDIALOG_H

#include <QDialog>

namespace Ui {
class BillingDialog;
}

class QTimer;

class BillingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BillingDialog(QWidget *parent = 0);
    ~BillingDialog();

public slots:
    void updateDuration();
    void showTopupDialog();

private slots:
    void confirmExit();
    void onTimeout();
    void onSessionStopped();

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::BillingDialog *ui;
    QTimer* timer;
};

#endif // BILLINGDIALOG_H
