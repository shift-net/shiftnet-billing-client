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

signals:
    void finished();

private:
    Ui::SettingsWidget *ui;
};

#endif // SETTINGSWIDGET_H
