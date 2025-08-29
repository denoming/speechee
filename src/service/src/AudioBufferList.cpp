// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "speechee/AudioBufferList.hpp"

#include <gsl/gsl-lite.hpp>

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
    gsl_Assert(not empty());
    std::lock_guard lock{_guard};
    auto* buffer = gst_buffer_list_get(_bufferList, 0);
    gsl_Assert(buffer != nullptr);
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
    gsl_Assert(buffer != nullptr);
    GstMapInfo info;
    if (gst_buffer_map(buffer, &info, GST_MAP_WRITE) == TRUE) {
        memcpy(info.data, ptr, len);
        gst_buffer_unmap(buffer, &info);
        std::lock_guard lock{_guard};
        gst_buffer_list_insert(_bufferList, -1, buffer);
    }
}

} // namespace jar
