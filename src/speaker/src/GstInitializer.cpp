#include "speaker/GstInitializer.hpp"

#include <jarvisto/Logger.hpp>

#include <gst/gst.h>

namespace jar {

bool
GstInitializer::initialize()
{
    GError* error{nullptr};
    if (gst_init_check(nullptr, nullptr, &error) != TRUE) {
        LOGE("Initialization failed: code: <{}>, domain: <{}>, message: <{}>",
             error->code,
             error->domain,
             error->message);
        return false;
    }
    return true;
}

} // namespace jar