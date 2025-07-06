#pragma once

#include "async_logger.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace ricox {
class log_flush;
}

namespace ricox {
class manager {	 // manager will be a singleton class
   private:
	std::unordered_map<std::string, std::shared_ptr<logger>> logger_map;
	std::shared_ptr<logger> default_logger;
	std::mutex mtx;

	manager();
	~manager() = default;

	manager(const manager&) = delete;
	manager& operator=(const manager&) = delete;
	manager(manager&&) = delete;
	manager&& operator=(manager&&) = delete;

   public:
	static auto get_instance() -> manager&;
	auto get_logger(const std::string& logger_name) -> std::shared_ptr<logger>;
	auto get_default_logger() -> std::shared_ptr<logger>;

	auto create_logger(const std::string& name, const std::vector<std::shared_ptr<log_flush>>& flush) -> void;
	auto create_logger(const std::shared_ptr<logger> logger_) -> void;
	auto remove_logger(const std::string& logger_name) -> void;

	template <typename flush_type, typename... Args>
	auto create_logger(const std::string& name, Args&&... args) -> void {
		auto lock = std::unique_lock<std::mutex>(mtx);
		auto builder = std::make_unique<logger_builder>();
		builder->set_name(name);
		builder->add_flush<flush_type>(std::forward<Args>(args)...);
		logger_map.try_emplace(name, builder->build());
	}
};
}  // namespace ricox