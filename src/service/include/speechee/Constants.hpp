#pragma once

namespace jar {

/* Default number of threads to use in synthesis threads pool  */
static constexpr int64_t kSynthesisPoolThreads{2};

/* Default number of threads of HTTP speaker service */
static constexpr int64_t kHttpServiceThreads{2};

/* Default TCP port to listen by HTTP speaker service */
static constexpr int64_t kHttpServicePort{2350};

} // namespace jar