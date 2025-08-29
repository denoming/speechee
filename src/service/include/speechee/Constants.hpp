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

namespace jar {

/* Default number of threads to use in synthesis threads pool  */
static constexpr int64_t kSynthesisPoolThreads{2};

/* Default number of threads of HTTP speaker service */
static constexpr int64_t kHttpServiceThreads{2};

/* Default TCP port to listen by HTTP speaker service */
static constexpr int64_t kHttpServicePort{2350};

} // namespace jar