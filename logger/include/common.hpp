#pragma once

#include <stdint.h>

namespace ricox {
namespace common {
static constexpr size_t DEFAULT_BUFFER_SIZE = 104857600U; // 100MB

enum struct log_level : uint8_t { DEBUG = 0U, INFO = 1U, WARN = 2U, ERROR = 3U, FATAL = 4U };
}  // namespace common
}  // namespace ricox