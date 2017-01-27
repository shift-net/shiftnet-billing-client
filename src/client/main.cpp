#include "global.h"
#include "screenlocker.h"
#include "application.h"
#include "connection.h"

#include <QApplication>
#include <QSharedMemory>
#include <QFile>
#include <QBuffer>
#include <QDataStream>
#include <QDebug>

int main(int argc, char** argv)
{
    Application app(SNBC_SETTINGS_PATH, argc, argv);
    app.setApplicationName(SNBC_APP_NAME);
    app.setApplicationDisplayName(SNBC_APP_DISPLAY_NAME);
    app.setApplicationVersion(SNBC_VERSION_STR);

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

    // load stylesheet
    {
        QFile file(":/main.qss");
        file.open(QFile::ReadOnly);
        app.setStyleSheet(QLatin1String(file.readAll()));
    }

    return app.exec();
}
