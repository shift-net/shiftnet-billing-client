#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "screenlocker.h"

namespace Ui {
    class SettingsWidget;
}

class SettingsWidget : public ScreenLockerContentWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = 0);
    ~SettingsWidget();

private slots:
    void accept();
    void exit();

signals:
    void finished();

private:
    void showError(QWidget* widget, const QString& message);
    Ui::SettingsWidget *ui;
};

#endif // SETTINGSWIDGET_H
