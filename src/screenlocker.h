#ifndef SCREENLOCKER_H
#define SCREENLOCKER_H

#include <QWidget>

class ScreenLocker : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenLocker(QWidget *parent = 0);

signals:

public slots:
};

#endif // SCREENLOCKER_H