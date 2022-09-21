#pragma once

#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace jar {

class SpeakerService {
public:
    static SpeakerService&
    instance();

    const char*
    name() const;

    void
    parseArgs(int argc, char* argv[]);

    int
    run();

private:
    SpeakerService();

    void
    initialize();

    void
    finalize();

    void
    proceed();

    bool
    waitForTermination();

    void
    defineOptions(po::options_description& description);

    void
    handleHelp(const po::options_description& description);

private:
    bool _helpRequested;
    po::variables_map _options;
};

} // namespace jar