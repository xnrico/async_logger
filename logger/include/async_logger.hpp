#pragma once

#include "common.hpp"
#include "async_worker.hpp"
#include "log_flush.hpp"

#include <memory>
#include <string>
#include <format> // C++20 Format, only supported on gcc-13+
#include <vector>

namespace ricox {
class logger {
   private:
	std::string name;
	std::vector<std::shared_ptr<log_flush>> flush;
	async_worker worker;

	// callback function given to the worker, invoked when data is ready for consumer to read
	auto flush_callback(async_buffer& buffer) -> void;

   public:
	auto get_name() -> std::string_view;

	// template<typename... Args>
	// auto log(ricox::common::log_level level, std::format_string<Args...> format, Args&&... args);
	logger() = delete;
	logger(const std::string& name_, const std::vector<std::shared_ptr<log_flush>>& flush_);
	logger(std::string&& name_, std::vector<std::shared_ptr<log_flush>>&& flush_) noexcept;
	~logger() = default;
};

class logger_builder {	// must use unique pointer and exclusive access from single thread
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

	auto set_name(std::string_view name_) -> void;
	auto add_flush(std::shared_ptr<log_flush> flush_) -> void;
	auto build() -> std::shared_ptr<logger>;
};

}  // namespace ricox