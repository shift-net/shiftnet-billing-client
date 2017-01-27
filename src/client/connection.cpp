#include "connection.h"
#include "global.h"
#include "application.h"

#include <QWebSocket>
#include <QTimer>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>

Connection::Connection(QObject* parent)
    : QObject(parent)
    , socket(new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this))
    , timer(new QTimer(this))
{
    timer->setInterval(5000);
    timer->setTimerType(Qt::VeryCoarseTimer);
    timer->start();

    QObject::connect(timer, SIGNAL(timeout()), SLOT(connect()));
    QObject::connect(socket, SIGNAL(connected()), timer, SLOT(stop()));
    QObject::connect(socket, SIGNAL(disconnected()), timer, SLOT(start()));

    QObject::connect(socket, SIGNAL(connected()), SIGNAL(connected()));
    QObject::connect(socket, SIGNAL(disconnected()), SIGNAL(disconnected()));
    QObject::connect(socket, SIGNAL(textMessageReceived(QString)), SLOT(processTextMessage(QString)));
}

void Connection::connect()
{
    if (socket->state() != QAbstractSocket::UnconnectedState)
        return;

    const QString serverAddress = qApp->settings(SNBC_SK_SERVER_ADDRESS, SNBC_DEFAULT_SERVER_ADDRESS).toString();
    const int serverPort = qApp->settings(SNBC_SK_SERVER_PORT, SNBC_DEFAULT_SERVER_PORT).toInt();

    socket->open(QUrl(QString("ws://%1:%2").arg(serverAddress, QString::number(serverPort))));
}

void Connection::disconnect()
{
    socket->close();
}

bool Connection::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

void Connection::send(const QString& msgType, const QVariant& data)
{
    socket->flush();
}

void Connection::processTextMessage(const QString& jsonString)
{
    QString s = jsonString;
    QJsonParseError jsonParseError;
    QJsonDocument doc = QJsonDocument::fromJson(s.toUtf8(), &jsonParseError);
    // BUGGY!!!! kadang pesan sebelumnya ikut masuk juga
    if (!(jsonParseError.error == QJsonParseError::NoError && doc.isArray())) {
        QStringList ss = s.split("][");
        if (ss.isEmpty()) return;
        s = "[" + ss.last();
        doc = QJsonDocument::fromJson(s.toUtf8(), &jsonParseError);
        if (!(jsonParseError.error == QJsonParseError::NoError && doc.isArray())) return;
    }

    QVariantList list = doc.array().toVariantList();
    if (list.size() != 2) return;

    emit received(list.at(0).toString(), list.at(1));
}
