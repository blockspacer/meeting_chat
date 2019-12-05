#include "network_http_client.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>
#include <QObject>
#include "network_status_detector.h"
#include <QTime>

namespace core {

NetworkHttpClientWorker::NetworkHttpClientWorker(QObject* parent)
	: QObject(parent)
{
}

NetworkHttpClientWorker::~NetworkHttpClientWorker()
{
    if (_naMgr) {
        _naMgr->deleteLater();
    }
}

void NetworkHttpClientWorker::request(std::shared_ptr<NetworkRequest> request, std::shared_ptr<INetworkCallback> callback)
{
    if (!_naMgr) {
        _naMgr = new QNetworkAccessManager();
        connect(_naMgr, &QNetworkAccessManager::finished, this, &NetworkHttpClientWorker::finished);
    }
    qDebug() << "---------------------------- request id:" << request->requestId() << " ----------------------------";

    QUrl url;
    url.setScheme("http");
    url.setHost(request->host().c_str());
    url.setPath(request->path().c_str());
    if (!request->query().empty()) {
        url.setQuery(request->query().c_str());
    }
    if (request->port() != -1) {
        url.setPort(request->port());
    }

    QNetworkRequest req(url);

    for (auto header : request->header()) {
        req.setRawHeader(header.first.c_str(), header.second.c_str());
    }

    QNetworkReply* reply = nullptr;

    if (request->method() == NetworkMethod::POST) {
        QByteArray data((char *)request->data().data(), request->data().size());
        reply = _naMgr->post(req, data);
    } else if (request->method() == NetworkMethod::GET) {
        reply = _naMgr->get(req);
    } else if (request->method() == NetworkMethod::PUT) {
        QByteArray data((char *)request->data().data(), request->data().size());
        reply = _naMgr->put(req, data);
    } else if (request->method() == NetworkMethod::DELETE_RESOURCE) {
        reply = _naMgr->deleteResource(req);
    } else if (request->method() == NetworkMethod::HEAD) {
        reply = _naMgr->head(req);
    }

    QTime tm;
    tm.restart();
    //QEventLoop loop;

    QObject::connect(reply, &QNetworkReply::finished, [this, request, reply, callback]() {
        if (request && reply) {
            int64_t code = this->transformErrorCode(reply->error());
            std::unordered_map<std::string, std::string> header;
            for (auto pair : reply->rawHeaderPairs()) {
                header[pair.first.toStdString()] = pair.second.toStdString();
            }
            RequestResult status = code == 0 ? RequestResult::SUCCESS : RequestResult::FAILED;
            NetworkErrorType errorType = NetworkErrorType(code);
            QByteArray bytes = reply->readAll();
            qDebug() << " --> bytes:" << bytes;
            auto data = std::make_shared<std::vector<uint8_t>>(bytes.begin(), bytes.end());
            std::string contentType = header["Content-Type"];
            std::string errorDescription = reply->errorString().toStdString();
            std::shared_ptr<NetworkResponse> response = std::make_shared<NetworkResponse>(code,
                                                                                          header,
                                                                                          status,
                                                                                          data,
                                                                                          contentType,
                                                                                          errorType,
                                                                                          errorDescription);

            emit resultReady(request->requestId(), response, callback);
        }
    });

    //connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    //loop.exec();
    qDebug() << " --> event loop for reply elapsed:" << tm.elapsed();
}

void NetworkHttpClientWorker::finished(QNetworkReply* reply)
{
    qDebug() << " -->" << reply->errorString();
    qDebug() << " -->" << reply->rawHeaderPairs();
    reply->deleteLater();
}

int64_t NetworkHttpClientWorker::transformErrorCode(QNetworkReply::NetworkError error)
{
    switch (error)
    {
    case QNetworkReply::NoError:
        return 0;
    case QNetworkReply::ConnectionRefusedError:
        return 1;
    case QNetworkReply::RemoteHostClosedError:
        return 2;
    case QNetworkReply::HostNotFoundError:
        return 3;
    case QNetworkReply::TimeoutError:
        return 4;
    case QNetworkReply::OperationCanceledError:
        return 5;
    case QNetworkReply::SslHandshakeFailedError:
        return 6;
    case QNetworkReply::TemporaryNetworkFailureError:
        return 7;
    case QNetworkReply::NetworkSessionFailedError:
        return 8;
    case QNetworkReply::BackgroundRequestNotAllowedError:
        return 9;
    case QNetworkReply::TooManyRedirectsError:
        return 10;
    case QNetworkReply::InsecureRedirectError:
        return 11;
    case QNetworkReply::ProxyConnectionRefusedError:
        return 101;
    case QNetworkReply::ProxyConnectionClosedError:
        return 102;
    case QNetworkReply::ProxyNotFoundError:
        return 103;
    case QNetworkReply::ProxyTimeoutError:
        return 104;
    case QNetworkReply::ProxyAuthenticationRequiredError:
        return 105;
    case QNetworkReply::ContentAccessDenied:
        return 201;
    case QNetworkReply::ContentOperationNotPermittedError:
        return 202;
    case QNetworkReply::ContentNotFoundError:
        return 203;
    case QNetworkReply::AuthenticationRequiredError:
        return 204;
    case QNetworkReply::ContentReSendError:
        return 205;
    case QNetworkReply::ContentConflictError:
        return 206;
    case QNetworkReply::ContentGoneError:
        return 207;
    case QNetworkReply::InternalServerError:
        return 401;
    case QNetworkReply::OperationNotImplementedError:
        return 402;
    case QNetworkReply::ServiceUnavailableError:
        return 403;
    case QNetworkReply::ProtocolUnknownError:
        return 301;
    case QNetworkReply::ProtocolInvalidOperationError:
        return 302;
    case QNetworkReply::UnknownNetworkError:
        return 99;
    case QNetworkReply::UnknownProxyError:
        return 199;
    case QNetworkReply::UnknownContentError:
        return 299;
    case QNetworkReply::ProtocolFailure:
        return 399;
    case QNetworkReply::UnknownServerError:
        return 499;
    }
    return -1;
}

NetworkHttpClient::NetworkHttpClient()
{
    _worker = new NetworkHttpClientWorker(nullptr);
	_worker->moveToThread(&_thread);
    connect(&_thread, &QThread::finished, _worker, &QObject::deleteLater, Qt::QueuedConnection);
    connect(this, &NetworkHttpClient::_request, _worker, &NetworkHttpClientWorker::request, Qt::QueuedConnection);
    connect(_worker, &NetworkHttpClientWorker::resultReady, this, &NetworkHttpClient::handleResults, Qt::QueuedConnection);
    _thread.start();
}

NetworkHttpClient::~NetworkHttpClient()
{
    _thread.quit();
    _thread.wait();
	_worker->deleteLater();
}

void NetworkHttpClient::init()
{

}

void NetworkHttpClient::reset()
{

}

void NetworkHttpClient::request(const std::shared_ptr<NetworkRequest>& request, const std::shared_ptr<INetworkCallback>& callback)
{
    emit _request(request, callback);
}

bool NetworkHttpClient::isNetworkReachable()
{
    return NetworkStatusDetector::instance()->isOnline();
}

bool NetworkHttpClient::isAvailable()
{
    return true;
}

void NetworkHttpClient::cancelRequest(const std::shared_ptr<NetworkRequest>& /*request*/)
{

}

void NetworkHttpClient::handleResults(int64_t requestID, std::shared_ptr<NetworkResponse> response, std::shared_ptr<INetworkCallback> callback)
{
    if (callback) {
        if (response->status == RequestResult::SUCCESS) {
            callback->onSuccess(requestID, response);
        } else {
            callback->onFailure(requestID, response);
        }
    }
}

}
