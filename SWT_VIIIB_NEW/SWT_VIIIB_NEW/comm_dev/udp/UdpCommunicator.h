#ifndef UDPCOMMUNICATOR_H
#define UDPCOMMUNICATOR_H

#include "ICommunicator.h"
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <functional>  // 必须包含，用于 std::function

class UdpCommunicator : public ICommunicator
{
    Q_OBJECT

public:
    explicit UdpCommunicator(QObject *parent = nullptr);
    ~UdpCommunicator() override;

    // 接口实现
    bool open(const QVariantMap &params) override;
    bool isOpen() const override;
    void close() override;
    void sendAsync(const QByteArray &data) override;
    QByteArray sendAndReceive(const QByteArray &request,
                              int timeout_ms = 1500,
                              int maxRetries = 3) override;
    void setReceiveCallback(std::function<void(const QByteArray&)> callback) override;
    QString lastError() const override;
    QString protocolType() const override;;
    QVariantMap status() const override;;


    // UDP 特有：设置目标地址和端口
    void setTarget(const QString &host, quint16 port);

signals:
    void connected();      // 模拟：绑定成功
    void disconnected();   // 模拟：关闭
    void errorOccurred(const QString &error);

private slots:
    void onReadyRead();
    void onError(QAbstractSocket::SocketError error);
    void retrySend();

private:
    QUdpSocket      *m_socket = nullptr;
    QHostAddress     m_targetHost;
    quint16          m_targetPort = 0;
    QString          m_lastError;

    // 异步接收回调（关键成员变量，已声明）
    std::function<void(const QByteArray&)> m_receiveCallback = nullptr;

    // 发送重试机制
    QByteArray       m_lastSentData;
    int              m_sendRetryCount = 0;
    QTimer          *m_retryTimer = nullptr;

    // 统计信息（用于 status()）
    qint64           m_bytesSent = 0;
    qint64           m_bytesReceived = 0;

};

#endif // UDPCOMMUNICATOR_H
