#include "manager.hpp"

namespace ricox {
manager::manager() {
	auto lock = std::unique_lock<std::mutex>(mtx);

	auto builder = std::make_unique<logger_builder>();
	builder->set_name("default");
	builder->add_flush(std::make_shared<std_flush>());
	default_logger = builder->build();
}

auto manager::get_instance() -> manager& {	// static method for singleton class
	static auto mgr = manager{};			// lazy initialization
	return mgr;
}

auto manager::get_logger(const std::string& logger_name) -> std::shared_ptr<logger> {
	auto lock = std::unique_lock<std::mutex>(mtx);

	auto it = logger_map.find(logger_name);
	if (it == logger_map.end()) return std::shared_ptr<logger>{nullptr};

	return it->second;
}

auto manager::get_default_logger() -> std::shared_ptr<logger> { return default_logger; }

auto manager::create_logger(const std::shared_ptr<logger> logger) -> void {
	auto lock = std::unique_lock<std::mutex>(mtx);
	logger_map.try_emplace(std::string{logger->get_name()}, logger);  // return values are not needed here
}

auto manager::remove_logger(const std::string& logger_name) -> void {
	auto lock = std::unique_lock<std::mutex>(mtx);
	logger_map.erase(logger_name);	// no action if non-existent
}

}  // namespace ricox
