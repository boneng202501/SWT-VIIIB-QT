#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include "ProtocolTypes.h"
#include "ProtocolCodec.h"
#include "ICommunicator.h"
#include <QObject>

class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolHandler(ICommunicator *comm, QObject *parent = nullptr);
    ~ProtocolHandler();

    // 对外提供的业务接口
    void sendIOInit();

signals:
    void ioInitFinished(bool success);           // IO初始化完成
    void voltageRegResult(int errorCode, float realVoltage);
    void logMessage(const QString &msg);         // 统一日志

private slots:
    void onDataReceived(const QByteArray &rawData);

private:
    ICommunicator *m_comm = nullptr;
    ProtocolCodec  m_codec;
};

#endif // PROTOCOLHANDLER_H
