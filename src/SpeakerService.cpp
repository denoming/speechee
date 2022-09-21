#include "SpeakerService.hpp"

#include "common/Logger.hpp"
#include "common/LoggerInitializer.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include <iostream>
#include <memory>

namespace asio = boost::asio;

namespace jar {

SpeakerService&
SpeakerService::instance()
{
    static std::unique_ptr<SpeakerService> instance;
    if (!instance) {
        instance.reset(new SpeakerService);
    }
    return *instance;
}

SpeakerService::SpeakerService()
    : _helpRequested{false}
{
}

const char*
SpeakerService::name() const
{
    return "SpeakerService";
}

void
SpeakerService::parseArgs(int argc, char* argv[])
{
    po::options_description d{"Options"};
    defineOptions(d);
    po::store(po::parse_command_line(argc, argv, d), _options);
    if (_options.contains("help")) {
        handleHelp(d);
    }
}

int
SpeakerService::run()
{
    LoggerInitializer::initialize();

    if (_helpRequested) {
        return EXIT_SUCCESS;
    }

    try {
        initialize();
        proceed();
    } catch (const std::exception& e) {
        LOGE("Exception: {}", e.what());
        return EXIT_FAILURE;
    }
    finalize();
    return EXIT_SUCCESS;
}

void
SpeakerService::initialize()
{
    LOGI("Initialize <{}> application", name());
}

void
SpeakerService::finalize()
{
    LOGI("Finalize <{}> application", name());
}

void
SpeakerService::proceed()
{
    if (!waitForTermination()) {
        LOGE("Waiting for termination has failed");
    }
}

bool
SpeakerService::waitForTermination()
{
    asio::io_context context;
    boost::asio::signal_set signals{context, SIGINT, SIGTERM};
    signals.async_wait([&context](const auto& /*error*/, int /*signal*/) {
        if (!context.stopped()) {
            context.stop();
        }
    });
    return (context.run() > 0);
}

void
SpeakerService::defineOptions(po::options_description& description)
{
    description.add_options()("help,h", "Display help");
}

void
SpeakerService::handleHelp(const po::options_description& description)
{
    _helpRequested = true;

    std::cout << description << std::endl;
}

} // namespace jar

int
main(int argc, char* argv[])
{
    try {
        auto& service = jar::SpeakerService::instance();
        service.parseArgs(argc, argv);
        return service.run();
    } catch (...) {
        return EXIT_FAILURE;
    }
}
