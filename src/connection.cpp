#include "connection.h"
#include "global.h"

#include <QTcpSocket>
#include <QTimer>
#include <QSettings>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Connection* Connection::_self = 0;

Connection::Connection(QObject* parent)
    : QObject(parent)
    , _socket(new QTcpSocket(this))
    , _autoReconnectTimer(new QTimer(this))
{
    _self = this;

    connect(_socket, SIGNAL(readyRead()), SLOT(processReadyRead()));
    connect(_socket, SIGNAL(connected()), SIGNAL(connected()));
    connect(_socket, SIGNAL(disconnected()), SIGNAL(disconnected()));
    connect(_autoReconnectTimer, SIGNAL(timeout()), SLOT(connectToServer()));

    // Reconnect otomatis tiap 5 detik
    _autoReconnectTimer->setTimerType(Qt::VeryCoarseTimer);
    _autoReconnectTimer->start(3000);
}

void Connection::connectToServer()
{
    if (_socket->state() != QAbstractSocket::UnconnectedState)
        return;

    QSettings settings(SNBC_SETTINGS_PATH, QSettings::IniFormat);
    QString serverAddress = settings.value(SNBC_SK_SERVER_ADDRESS, SNBC_DEFAULT_SERVER_ADDRESS).toString();
    int serverPort = settings.value(SNBC_SK_SERVER_PORT, SNBC_DEFAULT_SERVER_PORT).toInt();

    _socket->connectToHost(serverAddress, serverPort);
}

void Connection::disconnectFromServer()
{
    if (!isConnected())
        return;

    _socket->disconnectFromHost();
}

bool Connection::isConnected() const
{
    return _socket->state() == QAbstractSocket::ConnectedState;
}

void Connection::notifyExit()
{
    send("EXIT", QJsonObject());
}

void Connection::requestLogin(const QString& id, const QString& password)
{
    QJsonObject obj;
    obj["id"] = id;
    obj["password"] = password;
    send("LOGIN", obj);
}

void Connection::processReadyRead()
{
    const QJsonDocument doc = QJsonDocument::fromJson(_socket->readAll());
    if (!doc.isArray()) {
        qWarning() << "Invalid json format";
        return;
    }

    const QJsonArray data = doc.array();
    if (data.size() < 2) {
        qWarning() << "array length less than 1";
        return;
    }

    const QString& type = data[0].toString();
    const QJsonValue value = data[1];

    if (type == "INIT") {
        QJsonObject data = value.toObject();
        QJsonObject client = data.value("client").toObject();
        QJsonObject company = data.value("company").toObject();

        QSettings settings(SNBC_SETTINGS_PATH, QSettings::IniFormat);
        settings.setValue(SNBC_SK_CLIENT_ID, client.value("id").toString());
        settings.setValue(SNBC_SK_CLIENT_PASSWORD, client.value("password").toString());
        settings.setValue(SNBC_SK_COMPANY_NAME, company.value("name").toString());
        settings.setValue(SNBC_SK_COMPANY_ADDRESS, company.value("address").toString());

        emit receiveInit();
    }
    else if (type == "SHUTDOWN") {
        emit receiveShutdown();
    }
    else if (type == "RESTART") {
        emit receiveRestart();
    }
    else if (type == "LOGIN") {
        emit receiveLogin(value.toObject());
    }
    else if (type == "SESSION_START") {
        emit receiveSessionStart(value.toObject());
    }
    else if (type == "SESSION_STOP") {
        emit receiveSessionStop();
    }

    qDebug() << type << value;
}

void Connection::send(const QString& method, const QJsonValue& value)
{
    QJsonArray data;
    data.append(method);
    data.append(value);

    _socket->write(QJsonDocument(data).toJson(QJsonDocument::Compact));
    _socket->flush();
}
