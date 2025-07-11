#include "common.hpp"
#include "async_logger.hpp"
#include "manager.hpp"

#include <map>

namespace ricox {
namespace common {
auto get_logger(const std::string& name) -> std::shared_ptr<logger> { return manager::get_instance().get_logger(name); }

auto get_default_logger() -> std::shared_ptr<logger> { return manager::get_instance().get_default_logger(); }

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

auto create_logger(const std::string& name, const std::vector<std::shared_ptr<log_flush>>& flush)
	-> std::shared_ptr<logger> {
	manager::get_instance().create_logger(name, flush);
	return get_logger(name);
}

auto create_logger(std::shared_ptr<logger> logger_) -> std::shared_ptr<logger> {
	manager::get_instance().create_logger(logger_);
	return get_logger(logger_->get_name());
}

auto remove_logger(const std::string& name) -> void { manager::get_instance().remove_logger(name); }

}  // namespace common
}  // namespace ricox