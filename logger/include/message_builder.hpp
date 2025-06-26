#pragma once

#include "common.hpp"

#include <chrono>
#include <string>
#include <thread>
#include <memory>

namespace std::chrono {
using time_t = time_point<system_clock>;
}

namespace ricox {
struct log_message {
   public:
	std::string file_name;
	std::string logger_name;
	std::string message;
	std::chrono::time_t log_time;
	std::thread::id thread_id;
	size_t line;  // line number from which the message was logged
	common::log_level log_level;

	log_message(const std::string& file_in, const std::string& logger_in, const std::string& msg_in, std::chrono::time_t time_in,
				std::thread::id thread_in, size_t line_in, common::log_level level_in);

	log_message() = delete;
	~log_message() = default;

	log_message(const log_message&) = default;
	log_message& operator=(const log_message&) = default;
	log_message(log_message&&) = default;
	// log_message&& operator=(log_message&&) = default;
};

class message_builder {
   private:
    std::weak_ptr<log_message> message;

   public:
    message_builder(const std::shared_ptr<ricox::log_message>& message_);
	message_builder() = delete;
	~message_builder() = default;

	message_builder(const message_builder&) = delete;
	message_builder& operator=(const message_builder&) = delete;
	message_builder(message_builder&&) = delete;
	message_builder&& operator=(message_builder&&) = delete;

	auto digest() -> std::string;
};
}  // namespace ricox