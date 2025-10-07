#pragma once

// #include <format>  // C++20 Format, only supported on gcc-13+
#include <fmt/format.h>

#include <iostream>
#include <memory>
#include <source_location>
#include <string>
#include <vector>

#include "async_worker.hpp"
#include "log_level.hpp"

namespace ricox {

// using std::format_string;
// using std::vformat;
// using std::format;

using fmt::format_string;
using fmt::vformat;
using fmt::format;

// Forward declaration of async_buffer and async_worker
class async_buffer;
class log_flush;

class logger {
 private:
  std::string name;
  std::vector<std::shared_ptr<log_flush>> flush;
  // async_worker_mcv worker;
  async_worker_lf worker;  // alternative worker using lock-free buffer

  // callback function given to the worker, invoked when data is ready for consumer to read
  auto flush_callback(async_buffer& buffer) -> void;                  // MPMC use
  auto flush_callback(lf_buffer<std::string, 4096>& buffer) -> void;  // SPSC use

  auto serialize(common::log_level level, const std::string& file, const size_t line, const std::string& text) -> void;

 public:
  logger() = delete;
  logger(const std::string& name_, const std::vector<std::shared_ptr<log_flush>>& flush_);
  logger(std::string&& name_, std::vector<std::shared_ptr<log_flush>>&& flush_);
  ~logger() = default;

  auto get_name() noexcept -> const std::string&;

  template <typename... Args>
  auto log(common::log_level level, const std::source_location location, format_string<Args...> format,
           Args&&... args) -> void {
    // log a message with given log level and format string
    auto text = fmt::format(format, std::forward<Args>(args)...);
    serialize(level, location.file_name(), location.line(), text);
  }

  template <typename... Args>
  auto debug(const std::source_location location, format_string<Args...> format, Args&&... args) -> void {
    // log a debug message
    log(common::log_level::DEBUG, location, format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  auto info(const std::source_location location, format_string<Args...> format, Args&&... args) -> void {
    // log an info message
    log(common::log_level::INFO, location, format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  auto warn(const std::source_location location, format_string<Args...> format, Args&&... args) -> void {
    // log a warning message
    log(common::log_level::WARN, location, format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  auto error(const std::source_location location, format_string<Args...> format, Args&&... args) -> void {
    // log an error message
    log(common::log_level::ERROR, location, format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  auto fatal(const std::source_location location, format_string<Args...> format, Args&&... args) -> void {
    // log a fatal message
    log(common::log_level::FATAL, location, format, std::forward<Args>(args)...);
  }
};

class logger_builder {  // must use unique pointer and exclusive access from single thread
 private:
  std::string name;
  std::vector<std::shared_ptr<log_flush>> flush;

 public:
  logger_builder() = default;
  ~logger_builder() = default;

  logger_builder(const logger_builder&) = delete;
  logger_builder& operator=(const logger_builder&) = delete;
  logger_builder(logger_builder&&) = delete;
  logger_builder&& operator=(logger_builder&&) = delete;

  auto set_name(const std::string& name_) noexcept -> void;
  auto add_flush(std::shared_ptr<log_flush> flush_) -> void;
  auto build() -> std::shared_ptr<logger>;

  template <typename flush_type, typename... Args>
  auto add_flush(Args&&... args) -> void {
    // add a flush object of type flush_type with given arguments
    static_assert(std::is_base_of<log_flush, flush_type>::value, "flush_type must be derived from log_flush");
    auto flush_ptr = std::make_shared<flush_type>(std::forward<Args>(args)...);
    add_flush(flush_ptr);
  }
};

}  // namespace ricox