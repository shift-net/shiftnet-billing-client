#ifndef MAINTENANCEDIALOG_H
#define MAINTENANCEDIALOG_H

#include <QDialog>

namespace Ui {
class MaintenanceDialog;
}

class QTimer;

class MaintenanceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaintenanceDialog(QWidget *parent = 0);
    ~MaintenanceDialog();

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::MaintenanceDialog *ui;
};

#endif // MAINTENANCEDIALOG_H
