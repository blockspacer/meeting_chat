#pragma once

#include <memory>
#include "network.hpp"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QThread>

namespace core {

class NetworkHttpClientWorker : public QObject
{
    Q_OBJECT
public:
    NetworkHttpClientWorker(QObject* parent);
    ~NetworkHttpClientWorker();

public slots:
    void request(std::shared_ptr<NetworkRequest> request, const std::shared_ptr<INetworkCallback> callback);

protected slots:
    void finished(QNetworkReply* reply);

signals:
    void resultReady(int64_t requestID, std::shared_ptr<NetworkResponse> response, std::shared_ptr<INetworkCallback> callback);

private:
    int64_t transformErrorCode(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager* _naMgr = nullptr;
};

class NetworkHttpClient : public QObject, public INetworkClient, public std::enable_shared_from_this<NetworkHttpClient>
{
    Q_OBJECT
public:
    NetworkHttpClient();
    ~NetworkHttpClient() override;

    void request(const std::shared_ptr<NetworkRequest>& request, const std::shared_ptr<INetworkCallback>& callback) override;
    void init() override;
    void reset() override;
    bool isNetworkReachable() override;
    bool isAvailable() override;
    void cancelRequest(const std::shared_ptr<NetworkRequest>& request) override;

signals:
    void _request(std::shared_ptr<NetworkRequest> request, std::shared_ptr<INetworkCallback> callback);

protected slots:
    void handleResults(int64_t requestID, std::shared_ptr<NetworkResponse> response, std::shared_ptr<INetworkCallback> callback);

private:
    QThread _thread;
};

}
