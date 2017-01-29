#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QSettings>

class Connection;
class QTimer;
class ScreenLocker;

class Application : public QApplication
{
    Q_OBJECT

public:
    explicit Application(const QString& settingsPath, int argc, char** argv);

    inline static Application* instance() { return static_cast<Application*>(QCoreApplication::instance()); }
    inline Connection* connection() const { return _connection; }
    inline void setSettings(const QString& k, const QVariant& v)
    { _settings.setValue(k, v); }
    inline QVariant settings(const QString& k, const QVariant& d = QVariant()) const
    { return _settings.value(k, d); }

    int exec();

    bool isConnected() const;
    void sendSessionStop();
    void sendGuestLogin(const QString& username, const QString& voucherCode);
    void sendMemberLogin(const QString& username, const QString& password, const QString& code);
    void sendTopup(const QString& code);

public slots:
    void shutdownSystem();
    void restartSystem();
    void maintenance();

signals:
    void connected();
    void disconnected();

    void initialized();
    void durationUpdated();

    void sessionStarted();
    void sessionStopped();
    void sessionTimeout();

    void guestLoginFailed(const QString& msg);
    void memberLoginFailed(const QString& field, const QString& message);

    void topupFailed(const QString& msg);
    void topupSuccess(int duration);

    void maintenanceStoppedRemotely();

private slots:
    void sendInit();
    void lockScreen();
    void showBillingDialog();

    void processMessage(const QString& type, const QVariant& data);
    void setScreenLockerOnTop();

private:
    QSettings _settings;
    Connection* _connection;
    ScreenLocker* _screenLocker;
    QTimer* _windowTopMostTimer;
};


#if defined(qApp)
    #undef qApp
#endif

#define qApp (static_cast<Application*>(QCoreApplication::instance()))

#endif // APPLICATION_H
