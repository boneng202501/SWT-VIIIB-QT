#include "ProtocolCodec.h"
#include <QDataStream>
#include <QDebug>

ProtocolCodec::ProtocolCodec(QObject *parent) : QObject(parent) {}

QByteArray ProtocolCodec::makeFrame(quint8 prefix, quint8 cmd, const QByteArray &body)
{
    QByteArray frame;
    frame.reserve(16 + body.size());
    frame.append(prefix);
    frame.append(cmd);
    frame.append(body);

    // 文档中多数响应是固定长度，这里先不补DD，由调用者保证长度
    return frame;
}

// ====================== 打包 ======================
QByteArray ProtocolCodec::packIOInit(const IOInitRequest &req)
{
    QByteArray ba(reinterpret_cast<const char*>(&req), sizeof(IOInitRequest));
    return ba;
}

QByteArray ProtocolCodec::packVoltageReg(const VoltageRegRequest &req)
{
    QByteArray ba(reinterpret_cast<const char*>(&req), sizeof(VoltageRegRequest));
    return ba;
}

QByteArray ProtocolCodec::packSampleControl(const SampleControlRequestHeader &header,
                                            const QByteArray &ioCommandsOrAutoParam)
{
    QByteArray ba(reinterpret_cast<const char*>(&header), sizeof(SampleControlRequestHeader));
    ba.append(ioCommandsOrAutoParam);
    return ba;
}

// 其他打包函数类似实现...

// ====================== 解析 ======================
std::optional<IOInitResponse> ProtocolCodec::parseIOInitResponse(const QByteArray &data)
{
    if (data.size() != 16 || static_cast<unsigned char>(data[0]) != 0xD4) return std::nullopt;
    IOInitResponse resp;
    memcpy(&resp, data.constData(), sizeof(IOInitResponse));
    return resp;
}

// 采样基本包解析（示例）
std::optional<SampleBasicPacket> ProtocolCodec::parseSampleBasicPacket(const QByteArray &data)
{
    if (data.size() != 72 || static_cast<unsigned char>(data[0]) != 0xB0) return std::nullopt;
    SampleBasicPacket pkt;
    memcpy(&pkt, data.constData(), sizeof(SampleBasicPacket));
    return pkt;
}

bool ProtocolCodec::parseSampleDataPacket(const QByteArray &data,
                                          quint8 &outChannel,
                                          quint32 &outPacketSeq,
                                          QByteArray &outData)
{
    if (data.size() < 8 || data[0] != 0xB1) return false;
    outChannel = data[1];
    outPacketSeq = *reinterpret_cast<const quint32*>(data.constData() + 2);
    quint16 validLen = *reinterpret_cast<const quint16*>(data.constData() + 6);
    if (validLen > data.size() - 8) return false;
    outData = data.mid(8, validLen);
    return true;
}
