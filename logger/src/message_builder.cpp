#include "message_builder.hpp"
#include <iomanip>
#include <sstream>

namespace ricox {

// Define ANSI escape codes as constants for readability
constexpr const char* RESET = "\033[0m";
constexpr const char* BOLD = "\033[1m";
constexpr const char* BLACK = "\033[30m";
constexpr const char* RED = "\033[31m";
constexpr const char* YELLOW = "\033[33m";
constexpr const char* BLUE = "\033[34m";
constexpr const char* MAGENTA = "\033[35m";

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
		ss << std::put_time(std::localtime(&in_time_t), "%I:%M:%S %p");
		auto time_str = ss.str();

		// colorize the output based on log level
		auto COLOR = RESET;
		switch (msg->log_level) {
			case common::log_level::DEBUG:
				COLOR = BLUE;
				break;
			case common::log_level::INFO:
				COLOR = BLACK;
				break;
			case common::log_level::WARN:
				COLOR = YELLOW;
				break;
			case common::log_level::ERROR:
				COLOR = MAGENTA;
				break;
			case common::log_level::FATAL:
				COLOR = RED;
				break;
			default:
				COLOR = RESET;	// fallback to reset if log level is unknown
				break;
		}

		// format the file name to only include the base name
		auto pos = msg->file_name.find_last_of("/\\");
		if (pos != std::string::npos) {
			msg->file_name = msg->file_name.substr(pos + 1);
		}

		out_str += std::format("{}[{}][Thread #{}][{}:{}][{}][{}]:\t{}{}\n", COLOR, time_str,
							   common::get_short_thread_id(msg->thread_id), msg->file_name, msg->line, msg->logger_name,
							   common::level_to_string(msg->log_level), msg->message, RESET);
	} else {
		out_str += "Invalid log message\n";
	}
	return out_str;
}

}  // namespace ricox