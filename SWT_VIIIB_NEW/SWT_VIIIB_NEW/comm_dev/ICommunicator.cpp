#include "ICommunicator.h"
#include "UdpCommunicator.h"   // ← 这里包含子类头文件
#include "TcpCommunicator.h"

ICommunicator* ICommunicator::create(const QString &protocol, QObject *parent)
{
    QString p = protocol.toUpper().trimmed();

    if (p == "UDP") {
        return new UdpCommunicator(parent);
    }
    else if (p == "TCP") {
        return new TcpCommunicator(parent);
    }
    else {
        qWarning() << "不支持的协议类型:" << protocol;
        return nullptr;
    }
}
