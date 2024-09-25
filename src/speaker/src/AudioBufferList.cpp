#include "speaker/AudioBufferList.hpp"

#include <boost/assert.hpp>

#include <stdexcept>

namespace jar {

AudioBufferList::AudioBufferList(const size_t maxChunkSize, const size_t initialSize)
    : _maxChunkSize{maxChunkSize}
{
    if (_bufferList = gst_buffer_list_new_sized(initialSize); _bufferList == nullptr) {
        throw std::runtime_error("Unable to create audio buffer list");
    }
}

AudioBufferList::~AudioBufferList()
{
    gst_buffer_list_unref(_bufferList);
}

void
AudioBufferList::push(const std::string_view audio) const
{
    const auto size = audio.size();
    size_t offset{0};
    while (offset < size) {
        const size_t len = std::min(_maxChunkSize, size - offset);
        push(&audio[offset], len);
        offset += len;
    }
}

GstBuffer*
AudioBufferList::pop() const
{
    BOOST_ASSERT(!empty());
    std::lock_guard lock{_guard};
    auto* buffer = gst_buffer_list_get(_bufferList, 0);
    BOOST_ASSERT(buffer != nullptr);
    buffer = gst_buffer_ref(buffer);
    gst_buffer_list_remove(_bufferList, 0, 1);
    return buffer;
}

size_t
AudioBufferList::size() const
{
    std::lock_guard lock{_guard};
    return gst_buffer_list_length(_bufferList);
}

bool
AudioBufferList::empty() const
{
    return (size() == 0);
}

void
AudioBufferList::push(const char* ptr, const size_t len) const
{
    auto* buffer = gst_buffer_new_allocate(nullptr, len, nullptr);
    BOOST_ASSERT(buffer != nullptr);
    GstMapInfo info;
    if (gst_buffer_map(buffer, &info, GST_MAP_WRITE) == TRUE) {
        memcpy(info.data, ptr, len);
        gst_buffer_unmap(buffer, &info);
        std::lock_guard lock{_guard};
        gst_buffer_list_insert(_bufferList, -1, buffer);
    }
}

} // namespace jar
