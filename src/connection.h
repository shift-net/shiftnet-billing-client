#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>

class QTcpSocket;
class QTimer;

class Connection : public QObject
{
    Q_OBJECT

public:
    Connection(QObject* parent);
    inline static Connection* instance() { return _self; }

    bool isConnected() const;

    void requestLogin(const QString& id, const QString& password);
    void requestStopSession();
    void notifyExit();

signals:
    void connected();
    void disconnected();

    void receiveInit();

    void receiveShutdown();
    void receiveRestart();

    void receiveSessionStart(const QJsonObject& data);
    void receiveSessionStop();

    void receiveLogin(const QJsonObject& data);

public slots:
    void connectToServer();
    void disconnectFromServer();

private slots:
    void processReadyRead();

private:
    void send(const QString& method, const QJsonValue& value);

    QTcpSocket* _socket;
    static Connection* _self;
    QTimer* _autoReconnectTimer;
};

#endif // CONNECTION_H
