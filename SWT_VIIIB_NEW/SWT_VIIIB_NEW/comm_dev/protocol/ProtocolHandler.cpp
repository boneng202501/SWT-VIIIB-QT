#include "ProtocolHandler.h"
#include <QDebug>

ProtocolHandler::ProtocolHandler(ICommunicator *comm, QObject *parent)
    : QObject(parent), m_comm(comm)
{
    if (m_comm) {
        connect(m_comm, &ICommunicator::dataReceived, this, &ProtocolHandler::onDataReceived);
    }

    // 调压超时定时器
    m_voltageTimeoutTimer = new QTimer(this);
    m_voltageTimeoutTimer->setSingleShot(true);
    connect(m_voltageTimeoutTimer, &QTimer::timeout, this, [this]() {
        if (m_waitingVoltageResult) {
            emit logMessage("❌ 调压超时，未收到 0xD2 结果包");
            m_waitingVoltageResult = false;
        }
    });
}

ProtocolHandler::~ProtocolHandler() = default;

void ProtocolHandler::sendIOInit()
{
    IOInitRequest req;
    req.prefix = 0xA4;
    req.cmd = 0x34;
    req.ioFunc[0] = 0x0000000F;  // 示例，可从 UI 或配置读取
    req.ioFunc[1] = 0x00000000;
    req.ioLevel[0] = 0x00000000;
    req.ioLevel[1] = 0x00000000;
    req.encoderSlave = 0;

    QByteArray frame = m_codec.packIOInit(req);
    m_comm->sendAsync(frame);

    emit logMessage("→ 已发送 IO 初始化命令 (0xA4 0x34)");
}

void ProtocolHandler::sendVoltageReg(const VoltageRegRequest &req)
{
    QByteArray frame = m_codec.packVoltageReg(req);
    m_comm->sendAsync(frame);

    emit logMessage(QString("→ 已发送调压命令 (0xA1 0x%1)，目标电压: %2V, 误差: ±%3V, 超时: %4s")
                        .arg(req.cmd, 2, 16, QChar('0'))
                        .arg(req.targetVoltage)
                        .arg(req.errorRange)
                        .arg(req.timeoutSec));

    // 启动超时计时
    m_waitingVoltageResult = true;
    m_voltageTimeoutTimer->start(req.timeoutSec * 1000);
}

void ProtocolHandler::onDataReceived(const QByteArray &rawData)
{
    if (rawData.isEmpty()) return;

    QString hex = rawData.toHex(' ').toUpper();
    emit logMessage("收到原始包 (" + QString::number(rawData.size()) + " 字节): " + hex.left(80));

    quint8 prefix = quint8(rawData[0]);

    // IO 初始化响应
    if (rawData.size() == 16 && prefix == 0xD4) {
        auto resp = m_codec.parseIOInitResponse(rawData);
        if (resp) {
            bool success = (resp->success == 0x00);
            emit ioInitFinished(success);
            emit logMessage(success ? "✅ IO 初始化成功" : "❌ IO 初始化失败");
        }
        return;
    }

    // 调压立即应答 (0xD1)
    if (rawData.size() == 16 && prefix == 0xD1) {
        VoltageRegAck ack;
        memcpy(&ack, rawData.constData(), sizeof(VoltageRegAck));

        bool success = (ack.success == 0x00);
        emit voltageRegAckReceived(success);
        emit logMessage(success ?
                            "✅ 调压命令接收成功 (0xD1)" :
                            QString("❌ 调压命令接收失败 (success=0x%1)").arg(ack.success, 2, 16, QChar('0')));
        return;
    }

    // 调压结果返回 (0xD2)
    if (rawData.size() == 16 && prefix == 0xD2) {
        VoltageRegResult result;
        memcpy(&result, rawData.constData(), sizeof(VoltageRegResult));

        m_voltageTimeoutTimer->stop();
        m_waitingVoltageResult = false;

        QString msg = QString("调压最终结果：\n"
                              "  errorCode = 0x%1 (%2)\n"
                              "  实际电压 = %3 V")
                          .arg(result.errorCode, 2, 16, QChar('0'))
                          .arg(result.errorCode == 0x00 ? "成功" :
                                   result.errorCode == 0x01 ? "超时" :
                                   result.errorCode == 0x02 ? "电源损坏" :
                                   result.errorCode == 0x03 ? "电源质量不好" :
                                   result.errorCode == 0x04 ? "调压函数代码错误" : "未知错误")
                          .arg(result.realVoltage, 0, 'f', 2);

        emit voltageRegResultReceived(result.errorCode, result.realVoltage);
        emit logMessage(msg);
        return;
    }

    // 未知包
    emit logMessage("收到未知包，前缀: 0x" + QString::number(prefix, 16).toUpper());
}
