#include "TcpCommunicator.h"
#include <QDebug>
#include <QThread>

TcpCommunicator::TcpCommunicator(QObject *parent)
    : ICommunicator(parent)
{
    m_socket = new QTcpSocket(this);
    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setSingleShot(true);

    connect(m_socket, &QTcpSocket::readyRead,    this, &TcpCommunicator::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred,this, &TcpCommunicator::onError);
    connect(m_socket, &QTcpSocket::connected,    this, &TcpCommunicator::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &TcpCommunicator::onDisconnected);
    connect(m_reconnectTimer, &QTimer::timeout,  this, &TcpCommunicator::tryReconnect);
}

TcpCommunicator::~TcpCommunicator()
{
    performClose();
    delete m_socket;
    m_socket = nullptr;
}

bool TcpCommunicator::open(const QVariantMap &params)
{
    QString ip   = params.value("ip",   "127.0.0.1").toString();
    quint16 port = params.value("port", 5000).toUInt();

    m_host = QHostAddress(ip);
    m_port = port;

    m_lastError.clear();
    resetReconnectState();

    m_socket->connectToHost(m_host, m_port);

    return m_socket->waitForConnected(5000);
}

bool TcpCommunicator::isOpen() const
{
    return m_socket && m_socket->state() == QAbstractSocket::ConnectedState;
}

void TcpCommunicator::close()
{
    performClose();
 //   return true;
}

void TcpCommunicator::performClose()
{
    m_reconnectTimer->stop();
    resetReconnectState();

    if (m_socket && m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
        if (!m_socket->waitForDisconnected(3000)) {
            m_socket->abort();
        }
    }
    m_buffer.clear();
    emit disconnected();
}

void TcpCommunicator::sendAsync(const QByteArray &data)
{
    if (!isOpen()) {
        emit errorOccurred("未连接，无法发送");
        return;
    }
    m_socket->write(data);
    m_socket->flush();
    m_bytesSent += data.size();
}

QByteArray TcpCommunicator::sendAndReceive(const QByteArray &request,
                                           int timeout_ms,
                                           int maxRetries)
{
    if (!isOpen()) {
        m_lastError = "未连接";
        emit errorOccurred(m_lastError);
        return {};
    }

    QByteArray response;

    for (int attempt = 0; attempt < maxRetries; ++attempt) {
        sendAsync(request);

        QEventLoop loop;
        QTimer timer;
        timer.setSingleShot(true);
        timer.start(timeout_ms);

        bool received = false;
        auto conn = connect(m_socket, &QTcpSocket::readyRead, [&]() {
            response += m_socket->readAll();
            received = true;
            loop.quit();
        });

        connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        loop.exec();
        disconnect(conn);

        if (received) {
            m_bytesReceived += response.size();
            return response;
        }

        QThread::msleep(100);
    }

    m_lastError = QString("超时未收到响应，已重试 %1 次").arg(maxRetries);
    emit errorOccurred(m_lastError);
    return {};
}

void TcpCommunicator::setReceiveCallback(std::function<void(const QByteArray&)> callback)
{
    m_receiveCallback = callback;
}

QString TcpCommunicator::lastError() const
{
    return m_lastError;
}

QString TcpCommunicator::protocolType() const
{
    return "TCP";
}

QVariantMap TcpCommunicator::status() const
{
    QVariantMap info;
    info["type"]             = protocolType();
    info["isOpen"]           = isOpen();
    info["host"]             = m_host.toString();
    info["port"]             = m_port;
    info["state"]            = m_socket ? m_socket->state() : -1;
    info["lastError"]        = m_lastError;
    info["reconnectAttempt"] = m_currentReconnectAttempt;
    info["bytesSent"]        = m_bytesSent;
    info["bytesReceived"]    = m_bytesReceived;
    return info;
}

// ─────────────── 槽函数 ───────────────

void TcpCommunicator::onReadyRead()
{
    if (!m_socket->bytesAvailable()) return;

    QByteArray data = m_socket->readAll();
    m_buffer.append(data);

    if (m_receiveCallback) {
        m_receiveCallback(m_buffer);
    }
    emit dataReceived(m_buffer);

    m_bytesReceived += data.size();
    // 实际项目中应在这里判断包完整性，再清空 m_buffer
}

void TcpCommunicator::onError(QAbstractSocket::SocketError error)
{
    m_lastError = m_socket->errorString();
    emit errorOccurred(m_lastError);
    qDebug() << "TCP 错误:" << m_lastError << "(code:" << error << ")";

    if (m_socket->state() == QAbstractSocket::UnconnectedState) {
        tryReconnect();
    }
}

void TcpCommunicator::onConnected()
{
    resetReconnectState();
    qDebug() << "TCP 已连接到" << m_host.toString() << ":" << m_port;
    emit connected();
}

void TcpCommunicator::onDisconnected()
{
    qDebug() << "TCP 连接断开";
    emit disconnected();

    if (m_reconnectEnabled) {
        tryReconnect();
    }
}

void TcpCommunicator::tryReconnect()
{
    if (!m_reconnectEnabled || m_currentReconnectAttempt >= m_maxReconnectAttempts) {
        m_lastError = "重连次数已达上限";
        emit errorOccurred(m_lastError);
        return;
    }

    m_currentReconnectAttempt++;
    emit reconnecting(m_currentReconnectAttempt, m_maxReconnectAttempts);

    qDebug() << "第" << m_currentReconnectAttempt << "次尝试重连...";
    m_socket->connectToHost(m_host, m_port);

    m_reconnectTimer->start(m_reconnectIntervalMs);
}

void TcpCommunicator::resetReconnectState()
{
    m_currentReconnectAttempt = 0;
    m_reconnectTimer->stop();
}
