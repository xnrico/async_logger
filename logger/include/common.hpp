#pragma once
#include "manager.hpp"

#include <stdint.h>
#include <cstddef>
#include <string>
#include <memory>

namespace ricox {
    class logger;
}

namespace ricox {
namespace common {
static constexpr size_t DEFAULT_BUFFER_SIZE = 104857600U;  // 100MB
enum struct log_level : uint8_t { DEBUG = 0U, INFO = 1U, WARN = 2U, ERROR = 3U, FATAL = 4U };

// Helper functions for users
auto get_logger(const std::string& name) -> std::shared_ptr<logger>;
auto get_default_logger() -> std::shared_ptr<logger>;

}  // namespace common
}  // namespace ricox