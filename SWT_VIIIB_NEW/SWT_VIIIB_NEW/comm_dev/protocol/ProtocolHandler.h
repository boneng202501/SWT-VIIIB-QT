#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include "ProtocolTypes.h"
#include "ProtocolCodec.h"
#include "ICommunicator.h"
#include <QObject>
#include <QTimer>

class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolHandler(ICommunicator *comm, QObject *parent = nullptr);
    ~ProtocolHandler();

    // 业务发送接口
    void sendIOInit();
    void sendVoltageReg(const VoltageRegRequest &req);

signals:
    void ioInitFinished(bool success);
    void voltageRegAckReceived(bool success);                  // 0xD1 立即应答
    void voltageRegResultReceived(quint8 errorCode, float realVoltage); // 0xD2 结果
    void logMessage(const QString &msg);

private slots:
    void onDataReceived(const QByteArray &rawData);

private:
    ICommunicator *m_comm = nullptr;
    ProtocolCodec  m_codec;

    // 调压相关状态（可扩展为命令队列）
    bool m_waitingVoltageResult = false;
    QTimer *m_voltageTimeoutTimer = nullptr;
};

#endif // PROTOCOLHANDLER_H
