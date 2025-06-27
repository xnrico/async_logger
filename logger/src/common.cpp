#include "common.hpp"

#include <map>

namespace ricox {
namespace common {
auto get_logger(const std::string& name) -> std::shared_ptr<logger> { return manager::get_instance().get_logger(name); }

auto get_default_logger() -> std::shared_ptr<logger> { return manager::get_instance().get_default_logger(); }

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

auto get_short_thread_id(std::thread::id id) -> int {
	static std::atomic<int> next_short_id{0};
	static std::map<std::thread::id, int> id_map;
	static std::mutex map_mutex;  // Protect access to id_map

	std::lock_guard<std::mutex> lock(map_mutex);
	auto it = id_map.find(id);
	if (it == id_map.end()) {
		// New thread ID, assign a short one
		int new_id = next_short_id.fetch_add(1);
		id_map[id] = new_id;
		return new_id;
	}
	return it->second;
}

}  // namespace common
}  // namespace ricox