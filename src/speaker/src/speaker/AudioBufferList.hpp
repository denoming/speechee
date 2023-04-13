#pragma once

#include <mutex>
#include <string_view>

#include <gst/gstbuffer.h>
#include <gst/gstbufferlist.h>

namespace jar {

class AudioBufferList {
public:
    explicit AudioBufferList(std::size_t initialSize = 16);

    explicit AudioBufferList(std::string_view audio);

    ~AudioBufferList();

    void
    push(std::string_view audio);

    [[nodiscard]] GstBuffer*
    pop() const;

    [[nodiscard]] std::size_t
    size() const;

    [[nodiscard]] bool
    empty() const;

private:
    void
    push(const char* ptr, std::size_t len);

private:
    mutable std::mutex _guard;
    GstBufferList* _bufferList;
};

} // namespace jar
