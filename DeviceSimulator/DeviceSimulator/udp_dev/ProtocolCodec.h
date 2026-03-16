#ifndef PROTOCOLCODEC_H
#define PROTOCOLCODEC_H

#include "ProtocolTypes.h"
#include <QObject>
#include <QByteArray>
#include <optional>

class ProtocolCodec : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolCodec(QObject *parent = nullptr);

    // ==================== 打包函数（上位机发送） ====================
    QByteArray packIOInit(const IOInitRequest &req);
    QByteArray packVoltageReg(const VoltageRegRequest &req);
    QByteArray packSampleControl(const SampleControlRequestHeader &header,
                                 const QByteArray &ioCommandsOrAutoParam);
    QByteArray packReadLog();
    QByteArray packAnalogOutput(quint8 daChannel, float voltage);
    QByteArray packCommStatus(quint8 status, quint16 randomCode = 0);

    // ==================== 解析函数（下位机返回） ====================
    std::optional<IOInitResponse> parseIOInitResponse(const QByteArray &data);
    std::optional<VoltageRegAck> parseVoltageRegAck(const QByteArray &data);
    std::optional<VoltageRegResult> parseVoltageRegResult(const QByteArray &data);
    std::optional<SampleBasicPacket> parseSampleBasicPacket(const QByteArray &data);

    // 采样数据包解析辅助（返回通道号 + 数据）
    bool parseSampleDataPacket(const QByteArray &data,
                               quint8 &outChannel,
                               quint32 &outPacketSeq,
                               QByteArray &outData);

    // 通用校验（前缀 + 长度 + 简单和校验，可扩展为CRC）
    static bool verifyFrame(const QByteArray &frame, quint8 expectedPrefix);

signals:
    void logProtocol(const QString &msg);   // 方便调试

private:
    QByteArray makeFrame(quint8 prefix, quint8 cmd, const QByteArray &body);
};

#endif // PROTOCOLCODEC_H
