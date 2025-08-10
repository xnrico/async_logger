#include "log_level.hpp"

namespace ricox {
namespace common {
auto level_to_string(log_level level) -> std::string {
	switch (level) {
		case log_level::DEBUG:
			return "DEBUG";
		case log_level::INFO:
			return "INFO";
		case log_level::WARN:
			return "WARN";
		case log_level::ERROR:
			return "ERROR";
		case log_level::FATAL:
			return "FATAL";
		default:
			return "UNKNOWN";
	}
}
}  // namespace common
}  // namespace ricox