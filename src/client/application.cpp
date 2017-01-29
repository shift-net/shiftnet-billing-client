#include "application.h"
#include "global.h"
#include "connection.h"
#include "screenlocker.h"
#include "billingdialog.h"
#include "maintenancedialog.h"

#include <QTimer>
#include <windows.h>

bool exit_window(int type)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

   // Get a token for this process.
   if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
      return false;

   // Get the LUID for the shutdown privilege.
   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

   tkp.PrivilegeCount = 1;  // one privilege to set
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

   // Get the shutdown privilege for this process.
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

   if (GetLastError() != ERROR_SUCCESS)
      return false;

   // Shut down the system and force all applications to close.
   if (!ExitWindowsEx(type | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED))
      return false;

   return true;
}


Application::Application(const QString& settingsPath, int argc, char** argv)
    : QApplication(argc, argv)
    , _settings(settingsPath, QSettings::IniFormat)
    , _connection(new Connection)
    , _screenLocker(0)
    , _windowTopMostTimer(new QTimer(this))
{
    connect(connection(), SIGNAL(connected()), SLOT(sendInit()));
    connect(connection(), SIGNAL(received(QString,QVariant)), SLOT(processMessage(QString,QVariant)));
    connect(connection(), SIGNAL(connected()), SIGNAL(connected()));
    connect(connection(), SIGNAL(disconnected()), SIGNAL(disconnected()));
    connect(_windowTopMostTimer, SIGNAL(timeout()), SLOT(setScreenLockerOnTop()));
}

void Application::sendInit()
{
    if (connection()->isConnected())
        connection()->send("init", _screenLocker && _screenLocker->isVisible() ? "ready" : "maintenance");
}

void Application::sendGuestLogin(const QString& username, const QString& voucherCode)
{
    if (connection()->isConnected())
        connection()->send("guest-login", QVariantList({ username, voucherCode }));
}

void Application::sendMemberLogin(const QString& username, const QString& password, const QString& code)
{
    if (connection()->isConnected())
        connection()->send("member-login", QVariantList({ username, password, code }));
}

void Application::sendSessionStop()
{
    if (connection()->isConnected())
        connection()->send("session-stop");
}

void Application::sendTopup(const QString& code)
{
    if (connection()->isConnected())
        connection()->send("user-topup", code);
}

void Application::processMessage(const QString& type, const QVariant& message)
{
    if (type == "init") {
        const QVariantMap data = message.toMap();
        const QVariantMap company = data.value("company").toMap();
        const QVariantMap client = data.value("client").toMap();

        // update settings
        setSettings(SNBC_SK_COMPANY_NAME, company.value("name"));
        setSettings(SNBC_SK_COMPANY_ADDRESS, company.value("address"));
        setSettings(SNBC_SK_CLIENT_ID, client.value("id"));
        setSettings(SNBC_SK_CLIENT_PASSWORD, client.value("password"));

        emit initialized();
    }

    // Login
    else if (type == "guest-login-failed") {
        emit guestLoginFailed(message.toString());
    }
    else if (type == "member-login-failed") {
        QStringList d = message.toStringList();
        emit memberLoginFailed(d.first(), d.last());
    }

    // Session
    else if (type == "session-start") {
        emit sessionStarted();
        QVariantMap m = message.toMap();
        setProperty("username", m.value("username"));
        setProperty("duration", m.value("duration"));
        showBillingDialog();
    }
    else if (type == "session-sync") {
        setProperty("duration", message);
        emit durationUpdated();
    }
    else if (type == "session-stop") {
        emit sessionStopped();
    }
    else if (type == "session-timeout") {
        emit sessionTimeout();
    }

    // system remote
    else if (type == "system-shutdown") {
        shutdownSystem();
    }
    else if (type == "system-restart") {
        restartSystem();
    }
    else if (type == "maintenance-remote-stop") {
        maintenanceStoppedRemotely();
    }

    // topup
    else if (type == "user-topup-success") {
        emit topupSuccess(message.toInt());
    }
    else if (type == "user-topup-failed") {
        emit topupFailed(message.toString());
    }
}

void Application::lockScreen()
{
    _windowTopMostTimer->start(500);
    _screenLocker->showFullScreen();
    _screenLocker->standBy();
}

void Application::showBillingDialog()
{
    _windowTopMostTimer->stop();
    _screenLocker->hide();
    _screenLocker->setContentWidget(0);

    BillingDialog dialog;
    dialog.exec();

    lockScreen();
}

int Application::exec()
{
    _screenLocker =  new ScreenLocker;
    lockScreen();

    connection()->connect();

    return QApplication::exec();
}

void Application::maintenance()
{
    _windowTopMostTimer->stop();
    _screenLocker->hide();

    if (connection()->isConnected())
        connection()->send("maintenance-start");

    MaintenanceDialog dialog;
    if (dialog.exec()) {
        if (connection()->isConnected())
            connection()->send("maintenance-stop");
    }

    _windowTopMostTimer->start(500);
    _screenLocker->standBy();
    _screenLocker->show();
}

bool Application::isConnected() const
{
    return connection()->isConnected();
}

void Application::shutdownSystem()
{
    exit_window(EWX_SHUTDOWN);
}

void Application::restartSystem()
{
    exit_window(EWX_REBOOT);
}

void Application::setScreenLockerOnTop()
{
    if (_screenLocker && _screenLocker->isVisible()) {
        HWND hScreenLocker = (HWND)_screenLocker->winId();
        HWND hCurWnd = ::GetForegroundWindow();
        DWORD dwMyID = ::GetCurrentThreadId();
        DWORD dwCurID = ::GetWindowThreadProcessId(hCurWnd, NULL);
        ::AttachThreadInput(dwCurID, dwMyID, TRUE);
        ::SetWindowPos(hScreenLocker, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        ::SetWindowPos(hScreenLocker, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        ::SetForegroundWindow(hScreenLocker);
        ::AttachThreadInput(dwCurID, dwMyID, FALSE);
        ::SetFocus(hScreenLocker);
        ::SetActiveWindow(hScreenLocker);
    }
}
