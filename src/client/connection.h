#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QVariant>

class QWebSocket;
class QTimer;
class Application;

class Connection : public QObject
{
    Q_OBJECT
public:
    friend class Application;

    Connection(QObject* parent = 0);

    bool isConnected() const;
    void send(const QString& msgType, const QVariant& data = QVariant());

public slots:
    void connect();
    void disconnect();

signals:
    void connected();
    void disconnected();
    void received(const QString& msgType, const QVariant& data);

private slots:
    void processTextMessage(const QString& message);

private:
    QWebSocket* socket;
    QTimer* timer;
};

#endif // CONNECTION_H
