#ifndef TCPCOMMUNICATOR_H
#define TCPCOMMUNICATOR_H

#include "ICommunicator.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QEventLoop>

class TcpCommunicator : public ICommunicator
{
    Q_OBJECT

public:
    explicit TcpCommunicator(QObject *parent = nullptr);
    ~TcpCommunicator() override;

    bool open(const QVariantMap &params) override;
    bool isOpen() const override;
    void close() override;
    void sendAsync(const QByteArray &data) override;
    QByteArray sendAndReceive(const QByteArray &request,
                              int timeout_ms = 1500,
                              int maxRetries = 3) override;
    void setReceiveCallback(std::function<void(const QByteArray&)> callback) override;
    QString lastError() const override;
    QString protocolType() const  override;
    QVariantMap status() const  override;

    // 重连配置
    void setReconnectEnabled(bool enabled) { m_reconnectEnabled = enabled; }
    void setMaxReconnectAttempts(int attempts) { m_maxReconnectAttempts = attempts; }
    void setReconnectInterval(int ms) { m_reconnectIntervalMs = ms; }

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString &error);
    void reconnecting(int attempt, int maxAttempts);

private slots:
    void onReadyRead();
    void onError(QAbstractSocket::SocketError error);
    void onConnected();
    void onDisconnected();
    void tryReconnect();

private:
    void performClose();
    void resetReconnectState();

    QTcpSocket      *m_socket = nullptr;
    QHostAddress     m_host;
    quint16          m_port = 0;
    QString          m_lastError;

    QByteArray       m_buffer;
    std::function<void(const QByteArray&)> m_receiveCallback = nullptr;

    bool             m_reconnectEnabled = true;
    int              m_maxReconnectAttempts = 5;
    int              m_reconnectIntervalMs = 3000;
    int              m_currentReconnectAttempt = 0;
    QTimer          *m_reconnectTimer = nullptr;

    // 统计信息（用于 status()）
    qint64           m_bytesSent = 0;
    qint64           m_bytesReceived = 0;
};

#endif // TCPCOMMUNICATOR_H
