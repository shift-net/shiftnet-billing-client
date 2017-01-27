#ifndef TOPUPDIALOG_H
#define TOPUPDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui { class TopupDialog; }

class TopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TopupDialog(QWidget *parent = 0);
    ~TopupDialog();

public slots:
    void accept();

private slots:
    void onTimeout();
    void onTopupFailed(const QString& msg);
    void onTopupSuccess(int duration);

private:
    void showError(const QString& msg);

    Ui::TopupDialog *ui;
    QTimer timer;
};

#endif // TOPUPDIALOG_H
