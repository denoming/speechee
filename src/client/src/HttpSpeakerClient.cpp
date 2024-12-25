#include "speechee/HttpSpeakerClient.hpp"

#include <jarvisto/network/Asio.hpp>
#include <jarvisto/network/Http.hpp>
#include <jarvisto/network/Worker.hpp>

#include <boost/assert.hpp>
#include <boost/json.hpp>

namespace json = boost::json;

namespace {

const std::string_view kContent{"application/json"};
const std::string_view kTextTarget{"/synthesize-text"};
const std::string_view kSsmlTarget{"/synthesize-ssml"};

std::string
getTextPayload(const std::string& text, const std::string& lang)
{
    json::object root;
    root["text"] = text;
    root["lang"] = lang;
    return json::serialize(root);
}

std::string
getSsmlPayload(const std::string& text, const std::string& lang)
{
    json::object root;
    root["ssml"] = text;
    root["lang"] = lang;
    return json::serialize(root);
}

} // namespace

namespace jar {

class HttpSpeakerClient::Impl final : public ISpeakerClient {
public:
    Impl(std::string host, std::string port)
        : _host{std::move(host)}
        , _port{std::move(port)}
        , _resolver{_worker.executor()}
    {
        _worker.start();
    }

    ~Impl() final
    {
        _worker.stop();
    }

    void
    synthesizeText(const std::string& text, const std::string& lang) final
    {
        if (text.empty() or lang.empty()) {
            throw std::invalid_argument{"Invalid arguments"};
        }
        auto stream = connect();
        std::ignore = http::write(stream, prepareTextRequest(text, lang));
        complete(stream);
    }

    void
    synthesizeSsml(const std::string& ssml, const std::string& lang) final
    {
        if (ssml.empty() or lang.empty()) {
            throw std::invalid_argument{"Invalid arguments"};
        }
        auto stream = connect();
        std::ignore = http::write(stream, prepareSsmlRequest(ssml, lang));
        complete(stream);
    }

private:
    beast::tcp_stream
    connect()
    {
        const auto results = _resolver.resolve(_host, _port);
        if (results.empty()) {
            throw std::runtime_error{"Unable to resolve given host address"};
        }
        beast::tcp_stream stream{_worker.executor()};
        stream.connect(results);
        return stream;
    }

    http::request<http::string_body>
    prepareTextRequest(const std::string& text, const std::string& lang)
    {
        http::request<http::string_body> req{http::verb::post, kTextTarget, kHttpVersion11};
        req.set(http::field::host, _host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, kContent);
        req.body() = getTextPayload(text, lang);
        req.prepare_payload();
        return req;
    }

    http::request<http::string_body>
    prepareSsmlRequest(const std::string& ssml, const std::string& lang)
    {
        http::request<http::string_body> req{http::verb::post, kSsmlTarget, kHttpVersion11};
        req.set(http::field::host, _host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, kContent);
        req.body() = getSsmlPayload(ssml, lang);
        req.prepare_payload();
        return req;
    }

    static void
    complete(beast::tcp_stream& stream)
    {
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        std::ignore = http::read(stream, buffer, res);
        stream.socket().shutdown(tcp::socket::shutdown_both);
        if (res.result() != http::status::ok) {
            throw std::runtime_error{"Unexpected response code"};
        }
    }

private:
    std::string _host;
    std::string _port;
    Worker _worker;
    tcp::resolver _resolver;
};

HttpSpeakerClient::HttpSpeakerClient(std::string host, std::string port)
    : _impl{std::make_unique<Impl>(std::move(host), std::move(port))}
{
}

HttpSpeakerClient::~HttpSpeakerClient() = default;

void
HttpSpeakerClient::synthesizeText(const std::string& text, const std::string& lang)
{
    BOOST_ASSERT(_impl);
    _impl->synthesizeText(text, lang);
}

void
HttpSpeakerClient::synthesizeSsml(const std::string& ssml, const std::string& lang)
{
    BOOST_ASSERT(_impl);
    _impl->synthesizeSsml(ssml, lang);
}

} // namespace jar