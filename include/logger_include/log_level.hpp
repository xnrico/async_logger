#pragma once
#include <stdint.h>
#include <string>

namespace ricox {
namespace common {
enum struct log_level : uint8_t { DEBUG = 0U, INFO = 1U, WARN = 2U, ERROR = 3U, FATAL = 4U };

// Helper functions for internal use
auto level_to_string(log_level level) -> std::string;
}  // namespace common
}  // namespace ricox