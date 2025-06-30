#pragma once

#include "manager.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <thread>

namespace ricox {
namespace common {
static constexpr size_t DEFAULT_BUFFER_SIZE = 104857600U;  // 100MB

// Helper to get a short, human-readable thread ID
auto get_short_thread_id(std::thread::id id) -> int;

// Helper functions for users
auto get_logger(const std::string& name) -> std::shared_ptr<logger>;
auto get_default_logger() -> std::shared_ptr<logger>;

}  // namespace common
}  // namespace ricox