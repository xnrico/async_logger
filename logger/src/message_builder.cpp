#include "message_builder.hpp"
#include <iomanip>
#include <sstream>

namespace ricox {

log_message::log_message(const std::string& file_in, const std::string& logger_in, const std::string& msg_in,
						 std::chrono::time_t time_in, std::thread::id thread_in, size_t line_in,
						 common::log_level level_in)
	: file_name(file_in),
	  logger_name(logger_in),
	  message(msg_in),
	  log_time(time_in),
	  thread_id(thread_in),
	  line(line_in),
	  log_level(level_in) {
	static_assert(true);
}

message_builder::message_builder(const std::shared_ptr<ricox::log_message>& message_) : message(message_) {}

auto message_builder::digest() -> std::string {
	auto out_str = std::string{};

	if (auto msg = message.lock()) {
		// separately process thread_id to convert it to string since it is not directly printable
		auto ss = std::stringstream{};

		auto in_time_t = std::chrono::system_clock::to_time_t(msg->log_time);
		ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
		auto time_str = ss.str();

		out_str += std::format("[{}][Thread #{}][{}:{}][{}][{}]: {}\n", time_str, common::get_short_thread_id(msg->thread_id),
							   msg->file_name, msg->line, msg->logger_name, common::level_to_string(msg->log_level),
							   msg->message);
	} else {
		out_str += "Invalid log message\n";
	}
	return out_str;
}

}  // namespace ricox