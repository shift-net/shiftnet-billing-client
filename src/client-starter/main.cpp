#include <QCoreApplication>
#include <QProcess>
#include <QDebug>

class AppStarter : public QCoreApplication
{
    Q_OBJECT
public:
    AppStarter(int argc, char** argv)
        : QCoreApplication(argc, argv)
    {
        process.setProgram(applicationDirPath().append("/ShiftNetBillingClient.exe"));
        connect(&process, SIGNAL(finished(int)), SLOT(onProcessExit()));
    }

    int exec()
    {
        startProcess();
        return QCoreApplication::exec();
    }

    void startProcess()
    {
        process.start();
    }

public slots:
    void onProcessExit()
    {
        startProcess();
    }

private:
    QProcess process;
    int chain;
};

int main(int argc, char **argv)
{
    return AppStarter(argc, argv).exec();
}

#include "main.moc"
