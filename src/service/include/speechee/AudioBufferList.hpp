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
