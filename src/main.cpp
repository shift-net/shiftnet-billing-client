#include <QApplication>
#include <QSharedMemory>
#include <QBuffer>
#include <QDataStream>
#include <QDebug>

#include "screenlocker.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName("ShiftNet Billing Client");

    // Simple single instance application mechanism with shared memory
    QSharedMemory appPidSharedMemory("shiftnet-billing-client.pid");
    if (appPidSharedMemory.attach()) {
        QBuffer buffer;
        qint64 pid;
        QDataStream in(&buffer);

        appPidSharedMemory.lock();
        buffer.setData((char*)appPidSharedMemory.constData(), appPidSharedMemory.size());
        buffer.open(QBuffer::ReadOnly);
        in >> pid;
        appPidSharedMemory.unlock();

        qWarning() << "ShiftNetBillingClient is already running with PID" << pid;
        return 0;
    }
    else {
        QBuffer buffer;
        buffer.open(QBuffer::WriteOnly);

        QDataStream out(&buffer);
        out << app.applicationPid();

        if (!appPidSharedMemory.create(buffer.size())) {
            qCritical() << "Unable to create shared memory segment. Error:" << appPidSharedMemory.errorString();
            return 1;
        }

        appPidSharedMemory.lock();
        char *to = (char*)appPidSharedMemory.data();
        const char *from = buffer.data().data();
        memcpy(to, from, qMin(appPidSharedMemory.size(), (int)buffer.size()));
        appPidSharedMemory.unlock();
    }

    ScreenLocker locker;
    locker.showFullScreen();

    return app.exec();
}
