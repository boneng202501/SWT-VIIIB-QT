#ifndef ICOMMUNICATOR_H
#define ICOMMUNICATOR_H

#include <QObject>
#include <QVariantMap>
#include <QByteArray>
#include <QString>
#include <functional>  // 用于 std::function
class ICommunicator : public QObject
{
    Q_OBJECT

public:
    explicit ICommunicator(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~ICommunicator() = default;
    // 只声明工厂方法
    static ICommunicator* create(const QString &protocol, QObject *parent = nullptr);
    // 核心虚函数接口（所有子类必须实现）
    virtual bool open(const QVariantMap &params) = 0;
    virtual bool isOpen() const = 0;
    virtual void close() = 0;
    virtual void sendAsync(const QByteArray &data) = 0;
    virtual QByteArray sendAndReceive(const QByteArray &request,
                                      int timeout_ms = 1500,
                                      int maxRetries = 3) = 0;
    virtual void setReceiveCallback(std::function<void(const QByteArray&)> callback) = 0;
    virtual QString lastError() const = 0;
    virtual QString protocolType() const = 0;
    virtual QVariantMap status() const = 0;

signals:
    void dataReceived(const QByteArray &data);
    void errorOccurred(const QString &error);
    void connected();
    void disconnected();
    void reconnecting(int attempt, int maxAttempts);


};

#endif // ICOMMUNICATOR_H
