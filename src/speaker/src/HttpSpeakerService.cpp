#include "speaker/HttpSpeakerService.hpp"

#include <jarvisto/Logger.hpp>

#include "speaker/ISpeaker.hpp"

#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/assert.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <boost/json.hpp>

#include <optional>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace sys = boost::system;
namespace json = boost::json;

using tcp = net::ip::tcp;

namespace {

using Message = std::tuple<std::string, std::string>;

std::optional<Message>
getMessage(const std::string& message)
{
    sys::error_code ec;
    json::value json = json::parse(message, ec);
    if (ec) {
        LOGD("Unable to parse message: {}", ec.what());
        return std::nullopt;
    }

    if (auto root = json.if_object(); root) {
        std::string value;
        if (auto jsValue = root->if_contains("value"); jsValue) {
            if (auto jsStr = jsValue->if_string(); jsStr) {
                value.assign(jsStr->begin(), jsStr->end());
            }
        }
        std::string lang{"en-US"};
        if (auto jsLang = root->if_contains("lang"); jsLang) {
            if (auto jsStr = jsLang->if_string(); jsStr) {
                lang.assign(jsStr->begin(), jsStr->end());
            }
        }
        if (value.empty()) {
            LOGD("Required value field is missing");
            return std::nullopt;
        } else {
            return std::make_tuple(std::move(value), std::move(lang));
        }
    }

    return std::nullopt;
}

} // namespace

namespace jar {

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    inline static constexpr std::chrono::seconds kReadTimeoutSec{5};
    inline static constexpr std::string_view kSynthesizeTextTarget{"/synthesize-text"};
    inline static constexpr std::string_view kSynthesizeSsmlTarget{"/synthesize-ssml"};

    explicit HttpSession(tcp::socket&& socket, ISpeaker& speaker)
        : _stream{std::move(socket)}
        , _speaker{speaker}
    {
    }

    void
    run()
    {
        net::dispatch(_stream.get_executor(),
                      beast::bind_front_handler(&HttpSession::doRead, shared_from_this()));
    }

private:
    void
    doRead()
    {
        _stream.expires_after(kReadTimeoutSec);

        http::async_read(_stream,
                         _buffer,
                         _request,
                         beast::bind_front_handler(&HttpSession::onRead, shared_from_this()));
    }

    void
    onRead(sys::error_code ec, std::size_t bytesTransferred)
    {
        if (ec == http::error::end_of_stream) {
            LOGD("EoS is received");
            return doClose();
        }

        if (ec) {
            LOGE("Error on read: {}", ec.what());
        } else {
            LOGD("Reading has succeeded: <{}> bytes", bytesTransferred);
            doWrite(doProcess(std::move(_request)));
        }
    }

    void
    doWrite(http::message_generator message)
    {
        beast::async_write(_stream,
                           std::move(message),
                           beast::bind_front_handler(&HttpSession::onWrite, shared_from_this()));
    }

    void
    onWrite(sys::error_code /*ec*/, std::size_t /*bytesTransferred*/)
    {
        doClose();
    }

    void
    doClose()
    {
        sys::error_code ec;
        _stream.socket().shutdown(tcp::socket::shutdown_send, ec);
    }

    http::message_generator
    doProcess(http::request<http::string_body>&& req)
    {
        const auto makeResponse = [version = req.version()](http::status status) {
            http::response<http::string_body> res{status, version};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            return res;
        };

        if (req.method() != http::verb::post) {
            LOGD("Unknown HTTP method: {}", req.method_string());
            return makeResponse(http::status::bad_request);
        }

        http::response<http::string_body> res;
        if (req.target() == kSynthesizeTextTarget) {
            if (auto msg = getMessage(req.body()); msg) {
                const auto& [value, lang] = *msg;
                _speaker.synthesizeText(value, lang);
                res = makeResponse(http::status::ok);
            } else {
                res = makeResponse(http::status::bad_request);
            }
            return res;
        }
        if (req.target() == kSynthesizeSsmlTarget) {
            if (auto msg = getMessage(req.body()); msg) {
                const auto& [value, lang] = *msg;
                _speaker.synthesizeSsml(value, lang);
                res = makeResponse(http::status::ok);
            } else {
                res = makeResponse(http::status::bad_request);
            }
            return res;
        }
        LOGD("Unknown HTTP target: {}", req.target());
        return makeResponse(http::status::bad_request);
    }

private:
    beast::flat_buffer _buffer;
    http::request<http::string_body> _request;
    beast::tcp_stream _stream;
    ISpeaker& _speaker;
};

class HttpListener : public std::enable_shared_from_this<HttpListener> {
public:
    using Ptr = std::shared_ptr<HttpListener>;

    HttpListener(net::io_context& ioc, tcp::endpoint endpoint, ISpeaker& speaker)
        : _acceptor{ioc}
        , _endpoint{std::move(endpoint)}
        , _speaker{speaker}
    {
    }

    [[nodiscard]] std::uint16_t
    port() const
    {
        return _endpoint.port();
    }

    [[nodiscard]] bool
    listen()
    {
        if (not doListen()) {
            LOGE("Unable to start listening");
            return false;
        }
        doAccept();
        return true;
    }

private:
    bool
    doListen()
    {
        try {
            _acceptor.open(_endpoint.protocol());
            _acceptor.set_option(net::socket_base::reuse_address{true});
            _acceptor.bind(_endpoint);
            _acceptor.listen();
            return true;
        } catch (const sys::system_error& e) {
            LOGE("Exception: {}", e.what());
            return false;
        }
    }

    void
    doAccept()
    {
        _acceptor.async_accept(
            net::make_strand(_acceptor.get_executor()),
            beast::bind_front_handler(&HttpListener::onAccept, shared_from_this()));
    }

    void
    onAccept(sys::error_code ec, tcp::socket socket)
    {
        if (ec) {
            LOGE("Unable to accept connection: {}", ec.what());
            return;
        }

        std::make_shared<HttpSession>(std::move(socket), _speaker)->run();

        doAccept();
    }

private:
    tcp::endpoint _endpoint;
    tcp::acceptor _acceptor;
    ISpeaker& _speaker;
};

class HttpServer {
public:
    HttpServer(std::size_t concurrency, std::uint16_t port, ISpeaker& speaker)
        : _concurrency{concurrency}
        , _ioc{static_cast<int>(concurrency)}
        , _listener{std::make_shared<HttpListener>(_ioc, tcp::endpoint{tcp::v4(), port}, speaker)}
    {
    }

    ~HttpServer()
    {
        stop();
    }

    void
    start()
    {
        if (_ioc.stopped()) {
            LOGD("Restart inactive HTTP server");
            _ioc.restart();
        }

        LOGD("Start listening on <{}> port", _listener->port());
        if (!_listener->listen()) {
            LOGE("Error on start HTTP server listening");
        } else {
            LOGD("Start <{}> HTTP server threads", _concurrency);
            startThreads();
        }
    }

    void
    stop()
    {
        if (!_ioc.stopped()) {
            LOGD("Stop HTTP server");
            _ioc.stop();
        }

        if (!_threads.empty()) {
            LOGD("Stop <{}> HTTP server threads", _concurrency);
            stopThreads();
        }
    }

private:
    void
    startThreads()
    {
        _threads.reserve(_concurrency);
        for (std::size_t n = 0; n < _concurrency; ++n) {
            _threads.emplace_back(&HttpServer::threadFunc, this);
        }
    }

    void
    stopThreads()
    {
        for (std::size_t n = 0; n < _concurrency; ++n) {
            if (_threads[n].joinable()) {
                _threads[n].join();
            }
        }
        _threads.clear();
    }

    void
    threadFunc()
    {
        const auto guard = net::make_work_guard(_ioc);
        _ioc.run();
    }

private:
    std::size_t _concurrency;
    net::io_context _ioc;
    HttpListener::Ptr _listener;
    std::vector<std::thread> _threads;
};

class HttpSpeakerService::Impl {
public:
    explicit Impl(std::size_t concurrency, std::uint16_t port, ISpeaker& speaker)
        : _server{concurrency, port, speaker}
    {
    }

    void
    start()
    {
        _server.start();
    }

    void
    stop()
    {
        _server.stop();
    }

private:
    HttpServer _server;
};

HttpSpeakerService::HttpSpeakerService(std::size_t concurrency,
                                       std::uint16_t port,
                                       ISpeaker& speaker)
    : _impl{std::make_unique<Impl>(concurrency, port, speaker)}
{
}

HttpSpeakerService::~HttpSpeakerService() = default;

void
HttpSpeakerService::start()
{
    BOOST_ASSERT(_impl);
    _impl->start();
}

void
HttpSpeakerService::stop()
{
    BOOST_ASSERT(_impl);
    _impl->stop();
}

} // namespace jar
