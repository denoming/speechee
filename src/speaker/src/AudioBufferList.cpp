#include "speaker/AudioBufferList.hpp"

#include <boost/assert.hpp>

namespace jar {

AudioBufferList::AudioBufferList(std::size_t initialSize)
{
    _bufferList = gst_buffer_list_new_sized(initialSize);
    BOOST_ASSERT(_bufferList != nullptr);
}

AudioBufferList::AudioBufferList(std::string_view audio)
    : AudioBufferList{}
{
    push(audio);
}

AudioBufferList::~AudioBufferList()
{
    gst_buffer_list_unref(_bufferList);
}

void
AudioBufferList::push(std::string_view audio)
{
    const auto size = audio.size();
    const auto step = size / 10;

    std::size_t start{0};
    while (start < size) {
        std::size_t len = std::min(step, size - start);
        push(&audio[start], len);
        start += step;
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

std::size_t
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
AudioBufferList::push(const char* ptr, std::size_t len)
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