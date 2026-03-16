#include "MockDevice.h"
#include <QDateTime>
#include <QRandomGenerator>

MockDevice::MockDevice(QObject *parent) : QObject(parent)
{
    m_uploadTimer = new QTimer(this);
    m_uploadTimer->setSingleShot(false);
    connect(m_uploadTimer, &QTimer::timeout, this, &MockDevice::uploadNextBlock);
}

MockDevice::~MockDevice()
{
    stop();
}

bool MockDevice::start(quint16 port, const QString &protocolType, const QString &bindIp)
{
    stop();  // 先清理旧状态

    m_protocolType = protocolType.toUpper();

    if (m_protocolType == "UDP") {
        m_udpSocket = new QUdpSocket(this);
        if (m_udpSocket->bind(QHostAddress(bindIp), port)) {
            connect(m_udpSocket, &QUdpSocket::readyRead, this, &MockDevice::onUdpReadyRead);
            emit logMessage(QString("UDP 模拟器启动成功，监听 %1:%2").arg(bindIp).arg(port));
            return true;
        }
        emit logMessage("UDP 绑定失败: " + m_udpSocket->errorString());
        return false;
    }
    else if (m_protocolType == "TCP") {
        m_tcpServer = new QTcpServer(this);
        if (m_tcpServer->listen(QHostAddress(bindIp), port)) {
            connect(m_tcpServer, &QTcpServer::newConnection, this, &MockDevice::onTcpNewConnection);
            emit logMessage(QString("TCP 模拟器启动成功，监听 %1:%2").arg(bindIp).arg(port));
            return true;
        }
        emit logMessage("TCP 监听失败: " + m_tcpServer->errorString());
        return false;
    }
    else {
        emit logMessage("不支持的协议类型: " + protocolType);
        return false;
    }
}

void MockDevice::stop()
{
    if (m_udpSocket) {
        m_udpSocket->close();
        delete m_udpSocket;
        m_udpSocket = nullptr;
    }
    if (m_tcpServer) {
        m_tcpServer->close();
        delete m_tcpServer;
        m_tcpServer = nullptr;
    }
    if (m_tcpClient) {
        m_tcpClient->close();
        delete m_tcpClient;
        m_tcpClient = nullptr;
    }

    m_uploadTimer->stop();
    m_isSampling = false;
    emit logMessage("模拟器已停止 (" + m_protocolType + ")");
}

void MockDevice::sendToClient(const QByteArray &data)
{
      if (m_protocolType == "UDP") {
        if (m_udpSocket && !m_lastPeer.isNull()) {
            m_udpSocket->writeDatagram(data, m_lastPeer, m_lastPort);
        }
    }
    else if (m_protocolType == "TCP") {
        if (m_tcpClient && m_tcpClient->state() == QAbstractSocket::ConnectedState) {
            m_tcpClient->write(data);
            m_tcpClient->flush();
        }
    }
}

// UDP 接收
void MockDevice::onUdpReadyRead()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &m_lastPeer, &m_lastPort);

        processReceivedData(datagram);
    }
}

// TCP 新连接
void MockDevice::onTcpNewConnection()
{
    if (m_tcpClient) {
        m_tcpClient->close();
        delete m_tcpClient;
    }

    m_tcpClient = m_tcpServer->nextPendingConnection();
    if (m_tcpClient) {
        connect(m_tcpClient, &QTcpSocket::readyRead, this, &MockDevice::onTcpReadyRead);
        connect(m_tcpClient, &QTcpSocket::disconnected, this, &MockDevice::onTcpDisconnected);
        connect(m_tcpClient, &QTcpSocket::errorOccurred, this, &MockDevice::onTcpError);
        emit logMessage("TCP 客户端已连接: " + m_tcpClient->peerAddress().toString() + ":" + QString::number(m_tcpClient->peerPort()));
    }
}

// TCP 接收
void MockDevice::onTcpReadyRead()
{
    QByteArray data = m_tcpClient->readAll();
    processReceivedData(data);
}

// TCP 断开
void MockDevice::onTcpDisconnected()
{
    emit logMessage("TCP 客户端断开");
    if (m_tcpClient) {
        m_tcpClient->deleteLater();
        m_tcpClient = nullptr;
    }
}

// TCP 错误
void MockDevice::onTcpError(QAbstractSocket::SocketError error)
{
    emit logMessage("TCP 错误: " + m_tcpClient->errorString());
}

// 统一处理收到的数据（UDP/TCP 共用）


void MockDevice::processReceivedData(const QByteArray &datagram)
{
    QString hex = datagram.toHex(' ').toUpper();
    emit logMessage(QString("[%1] ← 收到 %2 字节: %3")
                        .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                        .arg(datagram.size())
                        .arg(hex.left(80)));

    if (datagram.size() < 2) return;

    quint8 prefix = quint8(datagram[0]);
    quint8 cmd    = quint8(datagram[1]);

    QByteArray reply;

    // IO 初始化
    if (prefix == 0xA4 && cmd == 0x34) {
        IOInitResponse resp;
        resp.prefix = 0xD4;
        resp.success = 0x00;
        reply = QByteArray(reinterpret_cast<const char*>(&resp), sizeof(IOInitResponse));
    }
    // 调压
    else if (prefix == 0xA1 && (cmd == 0x31 || cmd == 0x32)) {
        VoltageRegAck ack;
        ack.prefix = 0xD1;
        ack.success = 0x00;
        reply = QByteArray(reinterpret_cast<const char*>(&ack), sizeof(VoltageRegAck));

        // 模拟结果
        QTimer::singleShot(1000, this, [this]() {
            VoltageRegResult result;
            result.prefix = 0xD2;
            result.errorCode = 0x00;
            result.realVoltage = 220.5f;
            QByteArray resFrame(reinterpret_cast<const char*>(&result), sizeof(VoltageRegResult));
            sendToClient(resFrame);
            emit logMessage("→ 发送调压结果 0xD2 (成功, 220.5V)");
        });
    }
    // 采样控制（核心）
    else if (prefix == 0xA3 && cmd == 0x33) {
        QByteArray ack(16, 0xDD);
        ack[0] = 0xD3;
        ack[1] = 0x00;
        reply = ack;

        // 解析头部...
        // （后续自主判断、动作模拟、数据上传逻辑保持不变）
    }

    if (!reply.isEmpty()) {
        sendToClient(reply);
        emit logMessage(QString("→ 发送响应 %1 字节").arg(reply.size()));
    }
}
void MockDevice::uploadNextBlock()
{
    if (!m_isSampling) {
        emit logMessage("采样已停止，终止数据上传");
        return;
    }

    const int packetsPerBlock = 3;

    emit logMessage(QString("开始上传第 %1 块数据").arg(m_blockSeq));

    // 发送块信息 0xB8
    SampleBlockInfo block{};
    block.prefix = 0xB8;
    block.blockSeq = m_blockSeq;
    block.packetCountInBlock = packetsPerBlock;

    QByteArray blockFrame(reinterpret_cast<const char*>(&block), sizeof(SampleBlockInfo));
    sendToClient(blockFrame);
    emit logMessage(QString("→ 已发送 0xB8 块信息，块序号 %1，包数 %2").arg(m_blockSeq).arg(packetsPerBlock));

    // 发送数据包
    for (quint16 i = 0; i < packetsPerBlock; ++i) {
        QTimer::singleShot(i * 400, this, [=]() {
            QByteArray pkt(1400, 0x00);
            pkt[0] = 0xB1;
            pkt[1] = 1;  // 通道1
            quint32* seqPtr = reinterpret_cast<quint32*>(pkt.data() + 2);
            *seqPtr = i;
            quint16* lenPtr = reinterpret_cast<quint16*>(pkt.data() + 6);
            *lenPtr = 1392;

            // 模拟波形 + 噪声
            for (int j = 8; j < 1400; j += 2) {
                qreal sine = qSin(j * 0.05) * 1000;
                int noise = QRandomGenerator::global()->bounded(-100, 101);
                quint16 val = 2048 + static_cast<quint16>(sine + noise);
                pkt[j]   = val & 0xFF;
                pkt[j+1] = val >> 8;
            }

            sendToClient(pkt);
            emit logMessage(QString("→ 已发送 0xB1 数据包，通道1，序号 %1").arg(i));
        });
    }

    // 定时检查是否结束
    QTimer::singleShot(packetsPerBlock * 400 + 800, this, [this]() {
        if (m_blockSeq >= 4) {  // 示例：5块结束
            QByteArray endPkt(16, 0x00);
            endPkt[0] = 0xB2;
            sendToClient(endPkt);
            emit logMessage("→ 已发送 0xB2 采样结束包");
            m_isSampling = false;
            m_blockSeq = 0;
        } else {
            m_blockSeq++;
            uploadNextBlock();
        }
    });
}
void MockDevice::startDataUpload()
{
    if (!m_isSampling) {
        emit logMessage("采样未启动，无法上传数据");
        return;
    }

    emit logMessage("开始上传采样数据...");

    // 1. 发送 0xB0 基本包（采样基本信息）
    SampleBasicPacket basic{};
    basic.prefix = 0xB0;
    basic.errorCode = 0x00;
    basic.actualSampleRate = m_currentHeader.sampleRate;    // 从采样控制请求中取
    basic.pointsPerChannel[0] = 1024;                       // 示例：每个通道点数
    // 可以根据实际通道数填充更多
    // basic.pointsPerChannel[1] = 1024; ...

    QByteArray basicFrame(reinterpret_cast<const char*>(&basic), sizeof(SampleBasicPacket));
    sendToClient(basicFrame);

    emit logMessage("→ 已发送 0xB0 基本包");

    // 2. 延迟一小段时间后开始发送数据块（模拟设备准备）
    QTimer::singleShot(500, this, [this]() {
        m_currentActionIndex = 0;   // 重置动作序号
        uploadNextBlock();          // 开始第一个数据块
    });
}

// 其余函数（simulateTripComplete、startDataUpload、uploadNextBlock 等）保持不变
// 只需把原来的 m_socket->writeDatagram 改为 sendToClient()
