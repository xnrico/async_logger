#pragma once

#include "manager.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <thread>

namespace ricox {
class log_flush;

namespace common {

#define DEBUG_DEFAULT(format, ...) get_default_logger()->debug(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define INFO_DEFAULT(format, ...) get_default_logger()->info(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define WARN_DEFAULT(format, ...) get_default_logger()->warn(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define ERROR_DEFAULT(format, ...) get_default_logger()->error(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define FATAL_DEFAULT(format, ...) get_default_logger()->fatal(__FILE__, __LINE__, format, ##__VA_ARGS__)

#define DEBUG(name, format, ...) get_logger(name)->debug(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define INFO(name, format, ...) get_logger(name)->info(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define WARN(name, format, ...) get_logger(name)->warn(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define ERROR(name, format, ...) get_logger(name)->error(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define FATAL(name, format, ...) get_logger(name)->fatal(__FILE__, __LINE__, format, ##__VA_ARGS__)

static constexpr size_t DEFAULT_BUFFER_SIZE = 104857600U;  // 100MB

// Helper to get a short, human-readable thread ID
auto get_short_thread_id(std::thread::id id) -> int;

// Helper functions for users
auto get_logger(const std::string& name) -> std::shared_ptr<logger>;
auto get_default_logger() -> std::shared_ptr<logger>;
auto create_logger(const std::string& name, const std::vector<std::shared_ptr<log_flush>>& flush)
	-> std::shared_ptr<logger>;
auto create_logger(std::shared_ptr<logger> logger_) -> std::shared_ptr<logger>;
auto remove_logger(const std::string& name) -> void;

template <typename flush_type, typename... Args>
auto create_logger(const std::string& name, Args&&... args) -> std::shared_ptr<logger> {
	// Create a logger with a single flush type and given arguments
	static_assert(std::is_base_of<ricox::log_flush, flush_type>::value, "flush_type must be derived from log_flush");
	manager::get_instance().create_logger<flush_type>(name, std::forward<Args>(args)...);
	return get_logger(name);
}

}  // namespace common
}  // namespace ricox