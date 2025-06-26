#include "message_builder.hpp"

namespace ricox {

log_message::log_message(const std::string& file_in, const std::string& logger_in, const std::string& msg_in, std::chrono::time_t time_in,
						 std::thread::id thread_in, size_t line_in, common::log_level level_in)
	: file_name(file_in), logger_name(logger_in), message(msg_in), log_time(time_in), thread_id(thread_in), line(line_in), log_level(level_in) {
	static_assert(true);
}

message_builder::message_builder(const std::shared_ptr<ricox::log_message>& message_) : message(message_) {}

auto message_builder::digest() -> std::string {
	auto out_str = std::string{};

	return out_str;
}

}  // namespace ricox