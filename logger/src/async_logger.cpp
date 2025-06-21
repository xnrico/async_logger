#include "async_logger.hpp"

namespace ricox {
auto logger::get_name() -> std::string_view { return std::string_view{this->name}; }

auto logger_builder::set_name(std::string_view name_) -> void { this->name = name_; }

auto logger_builder::add_flush(std::shared_ptr<log_flush> flush_) -> void { this->flush.emplace_back(flush); }

auto logger_builder::build() -> std::shared_ptr<logger> {
	if (name.empty()) throw std::runtime_error("Empty logger name, cannot build a logger");
	if (flush.empty()) throw std::runtime_error("Empty flush vector, cannot build a logger");

	return std::make_shared<logger>(std::move(name), std::move(flush));
}
};	// namespace ricox