#include "SpeakerApplication.hpp"

#include "speech/Logger.hpp"
#include "speech/LoggerInitializer.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include <iostream>
#include <memory>

namespace io = boost::asio;

namespace jar {

SpeakerApplication&
SpeakerApplication::instance()
{
    static std::unique_ptr<SpeakerApplication> instance;
    if (!instance) {
        instance.reset(new SpeakerApplication);
    }
    return *instance;
}

SpeakerApplication::SpeakerApplication()
    : _helpRequested{false}
{
}

const char*
SpeakerApplication::name() const
{
    return "SpeakerApplication";
}

void
SpeakerApplication::parseArgs(int argc, char* argv[])
{
    po::options_description d{"Options"};
    defineOptions(d);
    po::store(po::parse_command_line(argc, argv, d), _options);
    if (_options.contains("help")) {
        handleHelp(d);
    }
}

int
SpeakerApplication::run()
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
SpeakerApplication::initialize()
{
    LOGI("Initialize <{}> application", name());
}

void
SpeakerApplication::finalize()
{
    LOGI("Finalize <{}> application", name());
}

void
SpeakerApplication::proceed()
{
    if (!waitForTermination()) {
        LOGE("Waiting for termination has failed");
    }
}

bool
SpeakerApplication::waitForTermination()
{
    io::io_context context;
    io::signal_set signals{context, SIGINT, SIGTERM};
    signals.async_wait([&context](const auto& /*error*/, int /*signal*/) {
        if (!context.stopped()) {
            context.stop();
        }
    });
    return (context.run() > 0);
}

void
SpeakerApplication::defineOptions(po::options_description& description)
{
    description.add_options()("help,h", "Display help");
}

void
SpeakerApplication::handleHelp(const po::options_description& description)
{
    _helpRequested = true;

    std::cout << description << std::endl;
}

} // namespace jar

int
main(int argc, char* argv[])
{
    try {
        auto& service = jar::SpeakerApplication::instance();
        service.parseArgs(argc, argv);
        return service.run();
    } catch (...) {
        return EXIT_FAILURE;
    }
}
