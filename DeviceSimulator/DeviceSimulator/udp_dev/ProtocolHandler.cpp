#include "ProtocolHandler.h"
#include <QDebug>

ProtocolHandler::ProtocolHandler(ICommunicator *comm, QObject *parent)
    : QObject(parent), m_comm(comm)
{
    if (m_comm) {
        connect(m_comm, &ICommunicator::dataReceived, this, &ProtocolHandler::onDataReceived);
    }
}

ProtocolHandler::~ProtocolHandler() = default;

void ProtocolHandler::sendIOInit()
{
    IOInitRequest req;
    req.prefix = 0xA4;
    req.cmd    = 0x34;
    req.ioFunc[0] = 0x0000000F;   // 示例：IO0~3 输入，其余输出
    req.ioFunc[1] = 0x00000000;
    req.ioLevel[0] = 0x00000000;
    req.ioLevel[1] = 0x00000000;
    req.encoderSlave = 0;

    QByteArray frame = m_codec.packIOInit(req);
    m_comm->sendAsync(frame);

    emit logMessage("→ 已发送 IO 初始化命令 (0xA4 0x34)");
}

void ProtocolHandler::onDataReceived(const QByteArray &rawData)
{
    if (rawData.size() == 16 && rawData[0] == 0xD4) {           // IO初始化响应
        auto resp = m_codec.parseIOInitResponse(rawData);
        if (resp) {
            bool success = (resp->success == 0x00);
            emit ioInitFinished(success);
            emit logMessage(success ? "✅ IO 初始化成功！" : "❌ IO 初始化失败");
        }
    }
    // 以后可以在这里继续添加 0xD1、0xD2、0xB0、0xB1 等解析
    else {
        emit logMessage("收到未知包: " + rawData.toHex(' ').toUpper());
    }
}
