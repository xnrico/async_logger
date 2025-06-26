#include "async_logger.hpp"

namespace ricox {
logger::logger(const std::string& name_, const std::vector<std::shared_ptr<log_flush>>& flush_)
	: name(name_), flush(flush_), worker([this](async_buffer& buffer) -> void { this->flush_callback(buffer); }) {}

// move construct version
logger::logger(std::string&& name_, std::vector<std::shared_ptr<log_flush>>&& flush_) noexcept
	: name(name_), flush(flush_), worker([this](async_buffer& buffer) -> void { this->flush_callback(buffer); }) {}

auto logger::flush_callback(async_buffer& buffer) -> void {
	// this runs asynchronously on a separate thread
	if (buffer.empty()) return;

	// flush buffer content to all flush targets
	for (const auto flush_target : flush) {
		flush_target->flush(std::string{buffer.read(buffer.get_readable_size())});
	}
}

auto logger::get_name() -> std::string_view { return std::string_view{this->name}; }

auto logger_builder::set_name(std::string_view name_) -> void { this->name = name_; }

auto logger_builder::add_flush(std::shared_ptr<log_flush> flush_) -> void { this->flush.emplace_back(flush_); }

auto logger_builder::build() -> std::shared_ptr<logger> {
	if (name.empty()) throw std::runtime_error("Empty logger name, cannot build a logger");
	if (flush.empty()) throw std::runtime_error("Empty flush vector, cannot build a logger");

	return std::make_shared<logger>(std::move(name), std::move(flush));
}
}  // namespace ricox