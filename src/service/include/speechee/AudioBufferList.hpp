#pragma once

#include <mutex>
#include <string_view>

#include <gst/gstbuffer.h>
#include <gst/gstbufferlist.h>

namespace jar {

class AudioBufferList {
public:
    explicit AudioBufferList(size_t maxChunkSize, size_t initialSize = 16);

    ~AudioBufferList();

    void
    push(std::string_view audio) const;

    [[nodiscard]] GstBuffer*
    pop() const;

    [[nodiscard]] size_t
    size() const;

    [[nodiscard]] bool
    empty() const;

private:
    void
    push(const char* ptr, size_t len) const;

private:
    mutable std::mutex _guard;
    size_t _maxChunkSize{};
    GstBufferList* _bufferList{};
};

} // namespace jar
