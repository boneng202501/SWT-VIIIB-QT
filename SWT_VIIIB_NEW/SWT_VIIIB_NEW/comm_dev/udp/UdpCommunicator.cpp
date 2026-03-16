#include "UdpCommunicator.h"
#include <QDebug>
#include <QThread>

UdpCommunicator::UdpCommunicator(QObject *parent)
    : ICommunicator(parent)
{
    m_socket = new QUdpSocket(this);

    connect(m_socket, &QUdpSocket::readyRead,    this, &UdpCommunicator::onReadyRead);
    connect(m_socket, &QUdpSocket::errorOccurred,this, &UdpCommunicator::onError);

    // 发送重试定时器
    m_retryTimer = new QTimer(this);
    m_retryTimer->setSingleShot(true);
    connect(m_retryTimer, &QTimer::timeout, this, &UdpCommunicator::retrySend);
}

UdpCommunicator::~UdpCommunicator()
{
    close();
    delete m_socket;
    m_socket = nullptr;
}

bool UdpCommunicator::open(const QVariantMap &params)
{
    QString ip   = params.value("ip",   "127.0.0.1").toString();
    quint16 port = params.value("port", 5000).toUInt();

    m_targetHost = QHostAddress(ip);
    m_targetPort = port;

    // UDP 绑定本地端口（AnyIPv4 随机端口）
    if (m_socket->bind(QHostAddress::AnyIPv4, 0)) {
        m_lastError.clear();
        emit connected();
        return true;
    }

    m_lastError = m_socket->errorString();
    emit errorOccurred(m_lastError);
    return false;
}

bool UdpCommunicator::isOpen() const
{
    return m_socket && m_socket->state() == QAbstractSocket::BoundState;
}

void UdpCommunicator::close()
{
    if (m_socket) {
        m_socket->close();
        m_socket->abort();
        m_retryTimer->stop();
        m_lastSentData.clear();
        m_sendRetryCount = 0;
    }
    emit disconnected();
 //   return true;
}

void UdpCommunicator::sendAsync(const QByteArray &data)
{
    if (!isOpen()) {
        emit errorOccurred("UDP 未绑定，无法发送");
        return;
    }

    qint64 sent = m_socket->writeDatagram(data, m_targetHost, m_targetPort);
    if (sent == -1) {
        m_lastError = m_socket->errorString();
        emit errorOccurred(m_lastError);

        // 发送失败 → 自动重试（最多 3 次）
        m_lastSentData = data;
        m_sendRetryCount = 3;
        m_retryTimer->start(500);
    } else {
        m_lastSentData.clear();
        m_sendRetryCount = 0;
        m_retryTimer->stop();
        m_bytesSent += data.size();
    }
}

QByteArray UdpCommunicator::sendAndReceive(const QByteArray &request,
                                           int timeout_ms,
                                           int maxRetries)
{
    if (!isOpen()) {
        m_lastError = "UDP 未绑定";
        emit errorOccurred(m_lastError);
        return {};
    }

    QByteArray response;

    for (int i = 0; i < maxRetries; ++i) {
        sendAsync(request);

        QEventLoop loop;
        QTimer timer;
        timer.setSingleShot(true);
        timer.start(timeout_ms);

        bool received = false;
        auto conn = connect(m_socket, &QUdpSocket::readyRead, [&]() {
            while (m_socket->hasPendingDatagrams()) {
                QByteArray datagram;
                datagram.resize(m_socket->pendingDatagramSize());
                m_socket->readDatagram(datagram.data(), datagram.size());
                response = datagram;  // 简单取第一个包（实际可匹配请求）
                received = true;
                loop.quit();
            }
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

void UdpCommunicator::setReceiveCallback(std::function<void(const QByteArray&)> callback)
{
    m_receiveCallback = callback;
}

QString UdpCommunicator::lastError() const
{
    return m_lastError;
}

QString UdpCommunicator::protocolType() const
{
    return "UDP";
}

QVariantMap UdpCommunicator::status() const
{
    QVariantMap info;
    info["type"]             = protocolType();
    info["isOpen"]           = isOpen();
    info["targetHost"]       = m_targetHost.toString();
    info["targetPort"]       = m_targetPort;
    info["lastError"]        = m_lastError;
    info["bytesSent"]        = m_bytesSent;
    info["bytesReceived"]    = m_bytesReceived;
    info["localPort"]        = m_socket ? m_socket->localPort() : -1;
    return info;
}

// ─────────────── 槽函数 ───────────────

void UdpCommunicator::onReadyRead()
{
    while (m_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        m_socket->readDatagram(datagram.data(), datagram.size());

        if (m_receiveCallback) {
            m_receiveCallback(datagram);
        }
        emit dataReceived(datagram);

        m_bytesReceived += datagram.size();
    }
}

void UdpCommunicator::onError(QAbstractSocket::SocketError error)
{
    m_lastError = m_socket->errorString();
    emit errorOccurred(m_lastError);
    qDebug() << "UDP 错误:" << m_lastError << "(code:" << error << ")";
}

void UdpCommunicator::retrySend()
{
    if (m_sendRetryCount > 0 && !m_lastSentData.isEmpty()) {
        qint64 sent = m_socket->writeDatagram(m_lastSentData, m_targetHost, m_targetPort);
        if (sent != -1) {
            m_lastSentData.clear();
            m_sendRetryCount = 0;
            m_retryTimer->stop();
        } else {
            m_sendRetryCount--;
            m_retryTimer->start(500);
        }
    }
}

void UdpCommunicator::setTarget(const QString &host, quint16 port)
{
    m_targetHost = QHostAddress(host);
    m_targetPort = port;
}
