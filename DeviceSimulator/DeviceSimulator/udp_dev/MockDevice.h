#ifndef MOCKDEVICE_H
#define MOCKDEVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QHostAddress>
#include "ProtocolTypes.h"
#include "ProtocolCodec.h"

class MockDevice : public QObject
{
    Q_OBJECT

public:
    explicit MockDevice(QObject *parent = nullptr);
    ~MockDevice() override;

    // 启动监听
    // protocolType: "UDP" 或 "TCP"
    bool start(quint16 port = 5000, const QString &protocolType = "UDP", const QString &bindIp = "0.0.0.0");

    void stop();

    // 手动模拟事件（供 UI 测试用）
    void simulateTripComplete();
    void simulateCloseComplete();
    void simulateFault();

signals:
    void logMessage(const QString &msg);

private slots:
    void onUdpReadyRead();
    void onTcpNewConnection();
    void onTcpReadyRead();
    void onTcpDisconnected();
    void onTcpError(QAbstractSocket::SocketError error);
    void processReceivedData(const QByteArray &datagram);


    void startDataUpload();
    void uploadNextBlock();

private:
    // 协议编解码
    ProtocolCodec m_codec;
    quint16 m_blockSeq = 0;
    // 通用状态
    QString m_protocolType;
    QHostAddress m_lastPeer;
    quint16 m_lastPort = 0;

    // UDP 专用
    QUdpSocket *m_udpSocket = nullptr;

    // TCP 专用
    QTcpServer *m_tcpServer = nullptr;
    QTcpSocket *m_tcpClient = nullptr;  // 当前连接的客户端

    // 采样状态
    bool m_isSampling = false;
    SampleControlRequestHeader m_currentHeader;
    QByteArray m_ioCommandsData;
    AutonomousParam m_autoParam;
    int m_currentActionIndex = 0;
    QTimer *m_uploadTimer = nullptr;

    // 模拟开关状态
    bool m_isClosed = false;
    bool m_coilEnergized = false;
    qint64 m_coilEnergizeTime = 0;

    // 通用发送函数（根据协议类型自动选择 socket）
    void sendToClient(const QByteArray &data);

    // 生成模拟波形
    QByteArray generateSimulatedWaveform(int points, int channels = 16);

    // 模拟单个动作
    void simulateSingleAction(bool isClose);

    void checkCoilTimeout();
    void checkContactAction();
};

#endif // MOCKDEVICE_H
