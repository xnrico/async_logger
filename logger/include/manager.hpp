#pragma once
#include "async_logger.hpp"

#include <string>
#include <unordered_map>

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
	auto create_logger(const std::shared_ptr<logger> logger) -> void;
	auto remove_logger(const std::string& logger_name) -> void;
};
}  // namespace ricox