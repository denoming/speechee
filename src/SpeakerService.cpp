#include "SpeakerService.hpp"

#include "speech/Logger.hpp"
#include "speech/LoggerInitializer.hpp"
#include "speech/Speaker.hpp"
#include "speech/SpeechSynthesizePool.hpp"
#include "speech/Player.hpp"
#include "speech/TextToSpeechClient.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include <iostream>
#include <memory>

namespace io = boost::asio;

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
    TextToSpeechClient client;
    SpeechSynthesizePool pool{client, 2};
    Player player;
    Speaker speaker{pool, player};

    player.initialize();

    if (!waitForTermination()) {
        LOGE("Waiting for termination has failed");
    }

    player.finalize();
}

bool
SpeakerService::waitForTermination()
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
