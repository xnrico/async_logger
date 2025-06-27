#pragma once

#include "async_worker.hpp"
#include "common.hpp"
#include "log_flush.hpp"

#include <format>  // C++20 Format, only supported on gcc-13+
#include <memory>
#include <string>
#include <vector>
#include <iostream>

namespace ricox {

namespace common {
enum struct log_level : uint8_t;  // forward declaration
}

class logger {
   private:
	std::string name;
	std::vector<std::shared_ptr<log_flush>> flush;
	async_worker worker;

	// callback function given to the worker, invoked when data is ready for consumer to read
	auto flush_callback(async_buffer& buffer) -> void;
	auto serialize(common::log_level level, const std::string& file, const size_t line, const std::string& text)
		-> void;

   public:
	logger() = delete;
	logger(const std::string& name_, const std::vector<std::shared_ptr<log_flush>>& flush_);
	logger(std::string&& name_, std::vector<std::shared_ptr<log_flush>>&& flush_) noexcept;
	~logger() = default;

	auto get_name() noexcept -> const std::string&;

	template <typename... Args>
	auto log(common::log_level level, const std::string& file, const size_t line, std::format_string<Args...> format,
			 Args&&... args) -> void {
		// log a message with given log level and format string
		auto text = std::format(format, std::forward<Args>(args)...);
		serialize(level, file, line, text);
	}
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

	auto set_name(const std::string& name_) noexcept -> void;
	auto add_flush(std::shared_ptr<log_flush> flush_) -> void;
	auto build() -> std::shared_ptr<logger>;
};

}  // namespace ricox